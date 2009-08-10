#!/bin/bash
#  memmon.ksh  09/06/24
#
#  Copyright (c) 7/2/2009
#  All rights reserved
#
#  Memmon
#
#  Modified:
#
#  This script checks the output from a 'ps -el' command against
#  previouse ps -el commands.  If the memory used continues to
#  increase over time the process is flagged as one that may have 
#  a memory leak.  This monitor works on the assumption that
#  processes will use a stable amount of memory over time.
#
#
#  OPTIONS:
#
#  -c Override default category (default - 'memmon.ksh')
#  -f Override default filter file (default - ./memfilt)
#  -p Override default priority (default - 3)
#  -g Override default growth count (default - 10)
#

export LC_TIME="C"  #set the time local so getdate works correctly

ME=`basename $0`
USAGE="Usage: $ME [-c category] [-g growth count] [-p priority]"
 
####################################################################
### This func is used to issue an error and quit; $1 is an err message
####################################################################
err_quit(){
    echo "$ME: $1" 1>&2
    exit 1
}
 
####################################################################
### This func is used to issue usage error
####################################################################
err_use(){
    echo $USAGE 1>&2
    exit 1
}
 
####################################################################
### This func is used collect base line information
####################################################################
collect_baseline_data(){
	ps -el|awk ' { print $4, $NF, $10, $10, 0 } ' >${PS_DATA}
}

####################################################################
### This func is used collect current information
####################################################################
collect_current_data(){
	ps -el|awk ' { print $4, $NF, $10, $10, 0 } ' >${CR_DATA}
}

#
# process the command line arguments if there are any.
#
 
Category=$ME
Priority=3
Growth_cnt=10
Filter_file=./memfilt
 
while getopts c:f:g:p: arg
do
    case $arg in
        c) Category=$OPTARG;;
        f) Filter_file=$OPTARG;;
        g) Growth_cnt=$OPTARG;;
        p) Priority=$OPTARG;;
       \?) err_use ;;
    esac
done


PS_DATA=/tmp/psdata_`uname -n`;export PS_DATA
CR_DATA=/tmp/crdata_`uname -n`;export CR_DATA

if [ -z "$Priority" ]; then
  err_quit "Must specify priority number with -p option"
fi
 
if [ "$Priority" -lt 1 -o "$Priority" -gt 10 ]; then
  err_quit "Invalid priority; 1 <=  p <= 10"
fi
 
if [ -z "$Category" ]; then
  err_quit "Must specify category when using -c option"
fi
 
if [ -z "$Growth_cnt" ]; then
  err_quit "Must specify a growth count when using -g option"
fi
 
####################################################################
### Get baseline data if it does not exist and we have been up
### at least 10 Minutes
####################################################################
if [ ! -s ${PS_DATA} ]
then
	Boottime=`getdate "\`who -r | awk '{print $3, $4, $5}'\` \`date +%Z\`"`
	Currenttime=`getdate now`
#############################################################################
# If Boottime is greater than Currenttime, assume the system was last booted
# last year
#############################################################################
     if [ $Boottime -gt $Currenttime ]
     then
        LASTYEAR="`date +%Z` `expr \`date +%Y\` - 1`"
        Boottime=`getdate "\`who -r | awk '{print $3, $4, $5}'\` $LASTYEAR"`
     fi
	Uptime=`expr $Currenttime - $Boottime`
	if [ $Uptime -ge 600 ]
	then
		collect_baseline_data
	else 
		# Wait until the system is up at least 10 minutes
		exit 0
	fi
fi

#
#get the current memory sizes used by all processes
#
collect_current_data

sort -n -o ${PS_DATA}1 -k 1 -k 1,1 ${PS_DATA}
sort -n -o ${CR_DATA}1 -k 1 -k 1,1 ${CR_DATA}

>${PS_DATA}2
sc_pid=0
b_pid=0
#typeset -L10 c_proc=
#typeset -L10 b_proc=
#declare -i c_proc=0
#declare -i b_proc=0

while read c_pid c_proc c_size c_isize c_growth  # read current PS info
do

	# skip Header line
	if [ "${c_pid}" = "PID" ]
	then
		continue
	fi

	if [ "${c_pid}" -eq "0" ] # skip the header line
	then
		continue
	fi

	# Test for filtering
	if [ -s ${Filter_file} ]
	then
		#skip if the proc is in the filter file
		grep ${c_proc} ${Filter_file} 2>&1 > /dev/null
		#grep ${c_proc} ${filter_file}
		if [ $? -eq 0 ]
		then
			continue
		fi
	fi

	while [ "${c_pid}" -gt "${b_pid}" ]
	do
		# read baseline PS info
		read -u3 b_pid b_proc b_size b_isize b_growth
		if [ $? -gt 0 ] #end of file
		then
			b_pid=0
			break
		fi
		if [ "${b_pid}" = "PID" ]
		then
			b_pid=0
			continue
		fi
	done

	if [ "${c_pid}" -eq "${b_pid}" ]
	then
		if [ ${c_size} -gt ${b_size} ]
		then
			b_growth=`expr $b_growth + 1`
			if [ "${b_growth}" -ge "${Growth_cnt}" ]
			then 
				#v_ms -p $Priority -c $Category -m "process <${c_pid} ${c_proc}> has grown ${b_growth} times, from ${b_isize} pages to ${c_size} pages, this process has a possible memory leak"
				echo "-p $Priority -c $Category -m \"process <${c_pid} ${c_proc}> has grown ${b_growth} times, from ${b_isize} pages to ${c_size} pages, this process has a possible memory leak\""
			fi
		elif [ ${c_size} -lt ${b_size} ]
		then
			b_growth=0
		fi
		#echo -e "${c_pid}\t${c_proc}\t${c_size}\t${b_isize}\t${b_growth}">> ${PS_DATA}2
		printf "%s\t%-20s\t%d\t%d\t%d\n" ${c_pid} ${c_proc} ${c_size} ${b_isize} ${b_growth}>> ${PS_DATA}2
	else
		#echo -e "${c_pid}\t${c_proc}\t${c_size}\t${c_isize}\t0">>${PS_DATA}2
		printf "%s\t%-20s\t%d\t%d\t0\n" ${c_pid} ${c_proc} ${c_size} ${c_isize} >>${PS_DATA}2
	fi
done < ${CR_DATA}1 3< ${PS_DATA}1
cp ${PS_DATA}2 ${PS_DATA}
rm ${CR_DATA}1 ${PS_DATA}1 ${PS_DATA}2 ${CR_DATA}

exit 0
