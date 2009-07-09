#include <string.h>

# define ID 257
# define MONTH 258
# define DAY 259
# define MERIDIAN 260
# define NUMBER 261
# define UNIT 262
# define MUNIT 263
# define SUNIT 264
# define ZONE 265
# define DAYZONE 266
# define AGO 267

	/* 	Steven M. Bellovin (unc!smb)			*/
	/*	Dept. of Computer Science			*/
	/*	University of North Carolina at Chapel Hill	*/
	/*	@(#)getdate.y	2.13	9/16/86 		*/

	/*							*/
	/*	This code is in the public domain and has no	*/
	/*	copyright					*/
	/*							*/
char pdent[] = "@(#) parse.c 3.6 09/07/06";

#include <stdio.h>
#include <sys/types.h>
#include <ctype.h>
#include <time.h>
#ifndef PC_SCO
#define	timezone	tmzn	/* ugly hack for obscure name clash */
#endif
#ifndef SGI_IRIX
#include <sys/timeb.h>
#else
  /*
   * Structure returned by ftime system call
   */
struct timeb {
    time_t     time;
    unsigned short millitm;
    short timezone;
    short dstflag;
};

#endif

#define daysec (24L*60L*60L)

	static int timeflag, zoneflag, dateflag, dayflag, relflag;
	static time_t relsec, relmonth;
	static int hh, mm, ss, merid, daylite;
	static int dayord, dayreq;
	static int month, day, year;
	static int ourzone;

#define AM 1
#define PM 2
#define DAYLIGHT 1
#define STANDARD 2
#define MAYBE    3
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#ifndef YYSTYPE
#define YYSTYPE int
#endif
YYSTYPE yylval, yyval;
typedef int yytabelem;
#include <stdio.h>
# define YYERRCODE 256
yytabelem yyexca[] ={
	-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 35
# define YYLAST 220
yytabelem yyact[]={

    12,    13,    21,     9,    14,    15,    16,    10,    11,    34,
    17,    39,    40,    19,    38,    37,    36,    30,    29,    28,
    26,    35,    31,    27,     8,     7,     6,     5,     4,     3,
     2,     1,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    32,    33,    22,    20,    18,     0,    23,    24,    25 };
yytabelem yypact[]={

-10000000,  -258,-10000000,-10000000,-10000000,-10000000,-10000000,  -257,-10000000,   -45,
-10000000,-10000000,  -241,   -21,-10000000,-10000000,-10000000,-10000000,-10000000,  -242,
-10000000,  -243,  -244,-10000000,-10000000,-10000000,   -22,-10000000,   -49,   -26,
-10000000,  -245,-10000000,-10000000,  -246,  -247,-10000000,  -249,-10000000,-10000000,
-10000000 };
yytabelem yypgo[]={

     0,    31,    30,    29,    28,    27,    26,    25,    24 };
yytabelem yyr1[]={

     0,     1,     1,     2,     2,     2,     2,     2,     2,     8,
     3,     3,     3,     3,     3,     3,     3,     4,     4,     6,
     6,     6,     5,     5,     5,     5,     5,     5,     7,     7,
     7,     7,     7,     7,     7 };
yytabelem yyr2[]={

     0,     0,     4,     3,     3,     3,     3,     3,     2,     3,
     5,     7,     9,     9,    11,    13,    13,     3,     3,     3,
     5,     5,     7,    11,     5,     9,     5,     7,     5,     5,
     5,     3,     3,     3,     5 };
yytabelem yychk[]={

-10000000,    -1,    -2,    -3,    -4,    -5,    -6,    -7,    -8,   261,
   265,   266,   258,   259,   262,   263,   264,   267,   260,    58,
   259,    47,   258,   262,   263,   264,   261,    44,   261,   261,
   261,    44,   260,   261,    58,    47,   261,   261,   261,   260,
   261 };
yytabelem yydef[]={

     1,    -2,     2,     3,     4,     5,     6,     7,     8,     9,
    17,    18,     0,    19,    31,    32,    33,    34,    10,     0,
    21,     0,    26,    28,    29,    30,    24,    20,    11,    22,
    27,     0,    12,    13,     0,     0,    25,    14,    23,    15,
    16 };
typedef struct { char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

char * yyreds[] =
{
	"-no such reduction-",
      "timedate : /* empty */",
      "timedate : timedate item",
      "item : tspec",
      "item : zone",
      "item : dtspec",
      "item : dyspec",
      "item : rspec",
      "item : nspec",
      "nspec : NUMBER",
      "tspec : NUMBER MERIDIAN",
      "tspec : NUMBER ':' NUMBER",
      "tspec : NUMBER ':' NUMBER MERIDIAN",
      "tspec : NUMBER ':' NUMBER NUMBER",
      "tspec : NUMBER ':' NUMBER ':' NUMBER",
      "tspec : NUMBER ':' NUMBER ':' NUMBER MERIDIAN",
      "tspec : NUMBER ':' NUMBER ':' NUMBER NUMBER",
      "zone : ZONE",
      "zone : DAYZONE",
      "dyspec : DAY",
      "dyspec : DAY ','",
      "dyspec : NUMBER DAY",
      "dtspec : NUMBER '/' NUMBER",
      "dtspec : NUMBER '/' NUMBER '/' NUMBER",
      "dtspec : MONTH NUMBER",
      "dtspec : MONTH NUMBER ',' NUMBER",
      "dtspec : NUMBER MONTH",
      "dtspec : NUMBER MONTH NUMBER",
      "rspec : NUMBER UNIT",
      "rspec : NUMBER MUNIT",
      "rspec : NUMBER SUNIT",
      "rspec : UNIT",
      "rspec : MUNIT",
      "rspec : SUNIT",
      "rspec : rspec AGO",
};
yytoktype yytoks[] =
{
	"ID",	257,
	"MONTH",	258,
	"DAY",	259,
	"MERIDIAN",	260,
	"NUMBER",	261,
	"UNIT",	262,
	"MUNIT",	263,
	"SUNIT",	264,
	"ZONE",	265,
	"DAYZONE",	266,
	"AGO",	267,
	"':'",	58,
	"','",	44,
	"'/'",	47,
	"-unknown-",	-1	/* ends search */
};
#endif /* YYDEBUG */

/* @(#)27       1.7.1.4  src/bos/usr/ccs/bin/yacc/yaccpar, cmdlang, bos430, 9737A_430 11/28/95 13:48:59 */
/*
 * COMPONENT_NAME: (CMDLANG) Language Utilities
 *
 * FUNCTIONS: yyparse
 * ORIGINS: 3
 */
/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#ifdef YYSPLIT
#   define YYERROR      return(-2)
#else
#   define YYERROR      goto yyerrlab
#endif
#ifdef YACC_MSG
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif
#include <nl_types.h>
nl_catd yyusercatd;
#endif
#define YYACCEPT        return(0)
#define YYABORT         return(1)
#ifndef YACC_MSG
#define YYBACKUP( newtoken, newvalue )\
{\
        if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
        {\
                yyerror( "syntax error - cannot backup" );\
                YYERROR;\
        }\
        yychar = newtoken;\
        yystate = *yyps;\
        yylval = newvalue;\
        goto yynewstate;\
}
#else
#define YYBACKUP( newtoken, newvalue )\
{\
        if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
        {\
                yyusercatd=catopen("yacc_user.cat", NL_CAT_LOCALE);\
                yyerror(catgets(yyusercatd,1,1,"syntax error - cannot backup" ));\
                YYERROR;\
        }\
        yychar = newtoken;\
        yystate = *yyps;\
        yylval = newvalue;\
        goto yynewstate;\
}
#endif
#define YYRECOVERING()  (!!yyerrflag)
#ifndef YYDEBUG
#       define YYDEBUG  1       /* make debugging available */
#endif

/*
** user known globals
*/
int yydebug;                    /* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG          (-10000000)

#ifdef YYSPLIT
#   define YYSCODE { \
                        extern int (*_yyf[])(); \
                        register int yyret; \
                        if (_yyf[yytmp]) \
                            if ((yyret=(*_yyf[yytmp])()) == -2) \
                                    goto yyerrlab; \
                                else if (yyret>=0) return(yyret); \
                   }
#endif

/*
** global variables used by the parser
*/
YYSTYPE yyv[ YYMAXDEPTH ];      /* value stack */
int yys[ YYMAXDEPTH ];          /* state stack */

YYSTYPE *yypv;                  /* top of value stack */
YYSTYPE *yypvt;                 /* top of value stack for $vars */
int *yyps;                      /* top of state stack */

int yystate;                    /* current state */
int yytmp;                      /* extra var (lasts between blocks) */

int yynerrs;                    /* number of errors */
int yyerrflag;                  /* error recovery flag */
int yychar;                     /* current input token number */

#ifdef __cplusplus
 #ifdef _CPP_IOSTREAMS
  #include <iostream.h>
  extern void yyerror (char *); /* error message routine -- iostream version */
 #else
  #include <stdio.h>
  extern "C" void yyerror (char *); /* error message routine -- stdio version */
 #endif /* _CPP_IOSTREAMS */
 extern "C" int yylex(void);        /* return the next token */
#endif /* __cplusplus */


/*
** yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
#ifdef __cplusplus
extern "C"
#endif /* __cplusplus */
int
yyparse()
{
        /*
        ** Initialize externals - yyparse may be called more than once
        */
        yypv = &yyv[-1];
        yyps = &yys[-1];
        yystate = 0;
        yytmp = 0;
        yynerrs = 0;
        yyerrflag = 0;
        yychar = -1;
#ifdef YACC_MSG
        yyusercatd=catopen("yacc_user.cat", NL_CAT_LOCALE);
#endif
        goto yystack;
        {
                register YYSTYPE *yy_pv;        /* top of value stack */
                register int *yy_ps;            /* top of state stack */
                register int yy_state;          /* current state */
                register int  yy_n;             /* internal state number info */

                /*
                ** get globals into registers.
                ** branch to here only if YYBACKUP was called.
                */
        yynewstate:
                yy_pv = yypv;
                yy_ps = yyps;
                yy_state = yystate;
                goto yy_newstate;

                /*
                ** get globals into registers.
                ** either we just started, or we just finished a reduction
                */
        yystack:
                yy_pv = yypv;
                yy_ps = yyps;
                yy_state = yystate;

                /*
                ** top of for (;;) loop while no reductions done
                */
        yy_stack:
                /*
                ** put a state and value onto the stacks
                */
#if YYDEBUG
                /*
                ** if debugging, look up token value in list of value vs.
                ** name pairs.  0 and negative (-1) are special values.
                ** Note: linear search is used since time is not a real
                ** consideration while debugging.
                */
                if ( yydebug )
                {
                        register int yy_i;

#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
                        cout << "State " << yy_state << " token ";
                        if ( yychar == 0 )
                                cout << "end-of-file" << endl;
                        else if ( yychar < 0 )
                                cout << "-none-" << endl;
#else
                        printf( "State %d, token ", yy_state );
                        if ( yychar == 0 )
                                printf( "end-of-file\n" );
                        else if ( yychar < 0 )
                                printf( "-none-\n" );
#endif /* defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
                        else
                        {
                                for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
                                        yy_i++ )
                                {
                                        if ( yytoks[yy_i].t_val == yychar )
                                                break;
                                }
#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
                                cout << yytoks[yy_i].t_name << endl;
#else
                                printf( "%s\n", yytoks[yy_i].t_name );
#endif /* defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
                        }
                }
#endif /* YYDEBUG */
                if ( ++yy_ps >= &yys[ YYMAXDEPTH ] )    /* room on stack? */
                {
#ifndef YACC_MSG
                        yyerror( "yacc stack overflow" );
#else
                        yyerror(catgets(yyusercatd,1,2,"yacc stack overflow" ));
#endif
                        YYABORT;
                }
                *yy_ps = yy_state;
                *++yy_pv = yyval;

                /*
                ** we have a new state - find out what to do
                */
        yy_newstate:
                if ( ( yy_n = yypact[ yy_state ] ) <= YYFLAG )
                        goto yydefault;         /* simple state */
#if YYDEBUG
                /*
                ** if debugging, need to mark whether new token grabbed
                */
                yytmp = yychar < 0;
#endif
                if ( ( yychar < 0 ) && ( ( yychar = yylex() ) < 0 ) )
                        yychar = 0;             /* reached EOF */
#if YYDEBUG
                if ( yydebug && yytmp )
                {
                        register int yy_i;

#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
                        cout << "Received token " << endl;
                        if ( yychar == 0 )
                                cout << "end-of-file" << endl;
                        else if ( yychar < 0 )
                                cout << "-none-" << endl;
#else
                        printf( "Received token " );
                        if ( yychar == 0 )
                                printf( "end-of-file\n" );
                        else if ( yychar < 0 )
                                printf( "-none-\n" );
#endif /* defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
                        else
                        {
                                for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
                                        yy_i++ )
                                {
                                        if ( yytoks[yy_i].t_val == yychar )
                                                break;
                                }
#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
                                cout << yytoks[yy_i].t_name << endl;
#else
                                printf( "%s\n", yytoks[yy_i].t_name );
#endif /* defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
                        }
                }
#endif /* YYDEBUG */
                if ( ( ( yy_n += yychar ) < 0 ) || ( yy_n >= YYLAST ) )
                        goto yydefault;
                if ( yychk[ yy_n = yyact[ yy_n ] ] == yychar )  /*valid shift*/
                {
                        yychar = -1;
                        yyval = yylval;
                        yy_state = yy_n;
                        if ( yyerrflag > 0 )
                                yyerrflag--;
                        goto yy_stack;
                }

        yydefault:
                if ( ( yy_n = yydef[ yy_state ] ) == -2 )
                {
#if YYDEBUG
                        yytmp = yychar < 0;
#endif
                        if ( ( yychar < 0 ) && ( ( yychar = yylex() ) < 0 ) )
                                yychar = 0;             /* reached EOF */
#if YYDEBUG
                        if ( yydebug && yytmp )
                        {
                                register int yy_i;

#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
                                cout << "Received token " << endl;
                                if ( yychar == 0 )
                                        cout << "end-of-file" << endl;
                                else if ( yychar < 0 )
                                        cout << "-none-" << endl;
#else
                                printf( "Received token " );
                                if ( yychar == 0 )
                                        printf( "end-of-file\n" );
                                else if ( yychar < 0 )
                                        printf( "-none-\n" );
#endif /* defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
                                else
                                {
                                        for ( yy_i = 0;
                                                yytoks[yy_i].t_val >= 0;
                                                yy_i++ )
                                        {
                                                if ( yytoks[yy_i].t_val
                                                        == yychar )
                                                {
                                                        break;
                                                }
                                        }
#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
                                        cout << yytoks[yy_i].t_name << endl;
#else
                                        printf( "%s\n", yytoks[yy_i].t_name );
#endif /* defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
                                }
                        }
#endif /* YYDEBUG */
                        /*
                        ** look through exception table
                        */
                        {
                                register int *yyxi = yyexca;

                                while ( ( *yyxi != -1 ) ||
                                        ( yyxi[1] != yy_state ) )
                                {
                                        yyxi += 2;
                                }
                                while ( ( *(yyxi += 2) >= 0 ) &&
                                        ( *yyxi != yychar ) )
                                        ;
                                if ( ( yy_n = yyxi[1] ) < 0 )
                                        YYACCEPT;
                        }
                }

                /*
                ** check for syntax error
                */
                if ( yy_n == 0 )        /* have an error */
                {
                        /* no worry about speed here! */
                        switch ( yyerrflag )
                        {
                        case 0:         /* new error */
#ifndef YACC_MSG
                                yyerror( "syntax error" );
#else
                                yyerror(catgets(yyusercatd,1,3,"syntax error" ));
#endif
                                goto skip_init;
                        yyerrlab:
                                /*
                                ** get globals into registers.
                                ** we have a user generated syntax type error
                                */
                                yy_pv = yypv;
                                yy_ps = yyps;
                                yy_state = yystate;
                                yynerrs++;
                        skip_init:
                        case 1:
                        case 2:         /* incompletely recovered error */
                                        /* try again... */
                                yyerrflag = 3;
                                /*
                                ** find state where "error" is a legal
                                ** shift action
                                */
                                while ( yy_ps >= yys )
                                {
                                        yy_n = yypact[ *yy_ps ] + YYERRCODE;
                                        if ( yy_n >= 0 && yy_n < YYLAST &&
                                                yychk[yyact[yy_n]] == YYERRCODE)                                        {
                                                /*
                                                ** simulate shift of "error"
                                                */
                                                yy_state = yyact[ yy_n ];
                                                goto yy_stack;
                                        }
                                        /*
                                        ** current state has no shift on
                                        ** "error", pop stack
                                        */
#if YYDEBUG
                                        if ( yydebug )
#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
                                            cout << "Error recovery pops state "
                                                 << (*yy_ps)
                                                 << ", uncovers state "
                                                 << yy_ps[-1] << endl;
#else
#       define _POP_ "Error recovery pops state %d, uncovers state %d\n"
                                                printf( _POP_, *yy_ps,
                                                        yy_ps[-1] );
#       undef _POP_
#endif /* defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
#endif
                                        yy_ps--;
                                        yy_pv--;
                                }
                                /*
                                ** there is no state on stack with "error" as
                                ** a valid shift.  give up.
                                */
                                YYABORT;
                        case 3:         /* no shift yet; eat a token */
#if YYDEBUG
                                /*
                                ** if debugging, look up token in list of
                                ** pairs.  0 and negative shouldn't occur,
                                ** but since timing doesn't matter when
                                ** debugging, it doesn't hurt to leave the
                                ** tests here.
                                */
                                if ( yydebug )
                                {
                                        register int yy_i;

#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
                                        cout << "Error recovery discards ";
                                        if ( yychar == 0 )
                                            cout << "token end-of-file" << endl;
                                        else if ( yychar < 0 )
                                            cout << "token -none-" << endl;
#else
                                        printf( "Error recovery discards " );
                                        if ( yychar == 0 )
                                                printf( "token end-of-file\n" );
                                        else if ( yychar < 0 )
                                                printf( "token -none-\n" );
#endif /* defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
                                        else
                                        {
                                                for ( yy_i = 0;
                                                        yytoks[yy_i].t_val >= 0;
                                                        yy_i++ )
                                                {
                                                        if ( yytoks[yy_i].t_val
                                                                == yychar )
                                                        {
                                                                break;
                                                        }
                                                }
#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
                                                cout << "token " <<
                                                    yytoks[yy_i].t_name <<
                                                    endl;
#else
                                                printf( "token %s\n",
                                                        yytoks[yy_i].t_name );
#endif /* defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
                                        }
                                }
#endif /* YYDEBUG */
                                if ( yychar == 0 )      /* reached EOF. quit */
                                        YYABORT;
                                yychar = -1;
                                goto yy_newstate;
                        }
                }/* end if ( yy_n == 0 ) */
                /*
                ** reduction by production yy_n
                ** put stack tops, etc. so things right after switch
                */
#if YYDEBUG
                /*
                ** if debugging, print the string that is the user's
                ** specification of the reduction which is just about
                ** to be done.
                */
                if ( yydebug )
#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
                        cout << "Reduce by (" << yy_n << ") \"" <<
                            yyreds[ yy_n ] << "\"\n";
#else
                        printf( "Reduce by (%d) \"%s\"\n",
                                yy_n, yyreds[ yy_n ] );
#endif /* defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
#endif
                yytmp = yy_n;                   /* value to switch over */
                yypvt = yy_pv;                  /* $vars top of value stack */
                /*
                ** Look in goto table for next state
                ** Sorry about using yy_state here as temporary
                ** register variable, but why not, if it works...
                ** If yyr2[ yy_n ] doesn't have the low order bit
                ** set, then there is no action to be done for
                ** this reduction.  So, no saving & unsaving of
                ** registers done.  The only difference between the
                ** code just after the if and the body of the if is
                ** the goto yy_stack in the body.  This way the test
                ** can be made before the choice of what to do is needed.
                */
                {
                        /* length of production doubled with extra bit */
                        register int yy_len = yyr2[ yy_n ];

                        if ( !( yy_len & 01 ) )
                        {
                                yy_len >>= 1;
                                yyval = ( yy_pv -= yy_len )[1]; /* $$ = $1 */
                                yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
                                        *( yy_ps -= yy_len ) + 1;
                                if ( yy_state >= YYLAST ||
                                        yychk[ yy_state =
                                        yyact[ yy_state ] ] != -yy_n )
                                {
                                        yy_state = yyact[ yypgo[ yy_n ] ];
                                }
                                goto yy_stack;
                        }
                        yy_len >>= 1;
                        yyval = ( yy_pv -= yy_len )[1]; /* $$ = $1 */
                        yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
                                *( yy_ps -= yy_len ) + 1;
                        if ( yy_state >= YYLAST ||
                                yychk[ yy_state = yyact[ yy_state ] ] != -yy_n )
                        {
                                yy_state = yyact[ yypgo[ yy_n ] ];
                        }
                }
                                        /* save until reenter driver code */
                yystate = yy_state;
                yyps = yy_ps;
                yypv = yy_pv;
        }
        /*
        ** code supplied by user is placed in this switch
        */

                switch(yytmp){

case 3:

		{timeflag++;} /*NOTREACHED*/ break;
case 4:

		{zoneflag++;} /*NOTREACHED*/ break;
case 5:

		{dateflag++;} /*NOTREACHED*/ break;
case 6:

		{dayflag++;} /*NOTREACHED*/ break;
case 7:

		{relflag++;} /*NOTREACHED*/ break;
case 9:

		{if (timeflag && dateflag && !relflag) year = yypvt[-0];
		else {timeflag++;hh = yypvt[-0]/100;mm = yypvt[-0]%100;ss = 0;merid = 24;}} /*NOTREACHED*/ break;
case 10:

		{hh = yypvt[-1]; mm = 0; ss = 0; merid = yypvt[-0];} /*NOTREACHED*/ break;
case 11:

		{hh = yypvt[-2]; mm = yypvt[-0]; merid = 24;} /*NOTREACHED*/ break;
case 12:

		{hh = yypvt[-3]; mm = yypvt[-1]; merid = yypvt[-0];} /*NOTREACHED*/ break;
case 13:

		{hh = yypvt[-3]; mm = yypvt[-1]; merid = 24;
		daylite = STANDARD; ourzone = yypvt[-0]%100 + 60*yypvt[-0]/100;} /*NOTREACHED*/ break;
case 14:

		{hh = yypvt[-4]; mm = yypvt[-2]; ss = yypvt[-0]; merid = 24;} /*NOTREACHED*/ break;
case 15:

		{hh = yypvt[-5]; mm = yypvt[-3]; ss = yypvt[-1]; merid = yypvt[-0];} /*NOTREACHED*/ break;
case 16:

		{hh = yypvt[-5]; mm = yypvt[-3]; ss = yypvt[-1]; merid = 24;
		daylite = STANDARD; ourzone = yypvt[-0]%100 + 60*yypvt[-0]/100;} /*NOTREACHED*/ break;
case 17:

		{ourzone = yypvt[-0]; daylite = STANDARD;} /*NOTREACHED*/ break;
case 18:

		{ourzone = yypvt[-0]; daylite = DAYLIGHT;} /*NOTREACHED*/ break;
case 19:

		{dayord = 1; dayreq = yypvt[-0];} /*NOTREACHED*/ break;
case 20:

		{dayord = 1; dayreq = yypvt[-1];} /*NOTREACHED*/ break;
case 21:

		{dayord = yypvt[-1]; dayreq = yypvt[-0];} /*NOTREACHED*/ break;
case 22:

		{month = yypvt[-2]; day = yypvt[-0];} /*NOTREACHED*/ break;
case 23:

		{month = yypvt[-4]; day = yypvt[-2]; year = yypvt[-0];} /*NOTREACHED*/ break;
case 24:

		{month = yypvt[-1]; day = yypvt[-0];} /*NOTREACHED*/ break;
case 25:

		{month = yypvt[-3]; day = yypvt[-2]; year = yypvt[-0];} /*NOTREACHED*/ break;
case 26:

		{month = yypvt[-0]; day = yypvt[-1];} /*NOTREACHED*/ break;
case 27:

		{month = yypvt[-1]; day = yypvt[-2]; year = yypvt[-0];} /*NOTREACHED*/ break;
case 28:

		{relsec +=  60L * yypvt[-1] * yypvt[-0];} /*NOTREACHED*/ break;
case 29:

		{relmonth += yypvt[-1] * yypvt[-0];} /*NOTREACHED*/ break;
case 30:

		{relsec += yypvt[-1];} /*NOTREACHED*/ break;
case 31:

		{relsec +=  60L * yypvt[-0];} /*NOTREACHED*/ break;
case 32:

		{relmonth += yypvt[-0];} /*NOTREACHED*/ break;
case 33:

		{relsec++;} /*NOTREACHED*/ break;
case 34:

		{relsec = -relsec; relmonth = -relmonth;} /*NOTREACHED*/ break;
}


        goto yystack;           /* reset registers in driver code */
}



static int mdays[12] =
	{31, 0, 31,  30, 31, 30,  31, 31, 30,  31, 30, 31};
#define epoch 1970

extern struct tm *localtime();
static time_t timeconv();

static time_t
dateconv(mm, dd, yy, h, m, s, mer, zone, dayflag)
int mm, dd, yy, h, m, s, mer, zone, dayflag;
{
	time_t tod, jdate;
	register int i;

	if (yy < 0) yy = -yy;
	if (yy < 1900) yy += 1900;
	mdays[1] = 28 + (yy%4 == 0 && (yy%100 != 0 || yy%400 == 0));
	if (yy < epoch || mm < 1 || mm > 12 ||
		dd < 1 || dd > mdays[--mm]) return (-1);
	jdate = dd-1;
        for (i=0; i<mm; i++) jdate += mdays[i];
	for (i = epoch; i < yy; i++) jdate += 365 + (i%4 == 0);
	jdate *= daysec;
	jdate += zone * 60L;
	if ((tod = timeconv(h, m, s, mer)) < 0) return (-1);
	jdate += tod;
	if (dayflag==DAYLIGHT || (dayflag==MAYBE&&localtime(&jdate)->tm_isdst))
		jdate += -1*60*60;
	return (jdate);
}

static	time_t daylcorr();
static time_t
dayconv(ord, day, now)
int ord, day; time_t now;
{
	register struct tm *loctime;
	time_t tod;

	tod = now;
	loctime = localtime(&tod);
	tod += daysec * ((day - loctime->tm_wday + 7) % 7);
	tod += 7*daysec*(ord<=0?ord:ord-1);
	return (daylcorr(tod, now));
}

static time_t
timeconv(hh, mm, ss, mer)
register int hh, mm, ss, mer;
{
	if (mm < 0 || mm > 59 || ss < 0 || ss > 59) return (-1);
	switch (mer) {
		case AM: if (hh < 1 || hh > 12) return(-1);
			 return (60L * ((hh%12)*60L + mm)+ss);
		case PM: if (hh < 1 || hh > 12) return(-1);
			 return (60L * ((hh%12 +12)*60L + mm)+ss);
		case 24: if (hh < 0 || hh > 23) return (-1);
			 return (60L * (hh*60L + mm)+ss);
		default: return (-1);
	}
}

static time_t
monthadd(sdate, relmonth)
time_t sdate, relmonth;
{
	struct tm *ltime;
	time_t dateconv();
	time_t daylcorr();
	int mm, yy;

	if (relmonth == 0) return 0;
	ltime = localtime(&sdate);
	mm = 12*ltime->tm_year + ltime->tm_mon + relmonth;
	yy = mm/12;
	mm = mm%12 + 1;
	return daylcorr(dateconv(mm, ltime->tm_mday, yy, ltime->tm_hour,
		ltime->tm_min, ltime->tm_sec, 24, ourzone, MAYBE), sdate);
}

static int lookup ();
static time_t
daylcorr(future, now)
time_t future, now;
{
	int fdayl, nowdayl;

	nowdayl = (localtime(&now)->tm_hour+1) % 24;
	fdayl = (localtime(&future)->tm_hour+1) % 24;
	return (future-now) + 60L*60L*(nowdayl-fdayl);
}

static char *lptr;

yylex()
{
	extern int yylval;
	int sign;
	register char c;
	register char *p;
	char idbuf[20];
	int pcnt;

	for (;;) {
		while (isspace(*lptr)) lptr++;

		if (isdigit(c = *lptr) || c == '-' || c == '+') {
			if (c== '-' || c == '+') {
				if (c=='-') sign = -1;
				else sign = 1;
				if (!isdigit(*++lptr)) {
					/* yylval = sign; return (NUMBER); */
					return yylex();	/* skip the '-' sign */
				}
			} else sign = 1;
			yylval = 0;
			while (isdigit(c = *lptr++)) yylval = 10*yylval + c - '0';
			yylval *= sign;
			lptr--;
			return (NUMBER);

		} else if (isalpha(c)) {
			p = idbuf;
			while (isalpha(c = *lptr++) || c=='.')
				if (p < &idbuf[sizeof(idbuf)-1])
					*p++ = c;
			*p = '\0';
			lptr--;
			return (lookup(idbuf));
		}

		else if (c == '(') {
			pcnt = 0;
			do {
				c = *lptr++;
				if (c == '\0') return(c);
				else if (c == '(') pcnt++;
				else if (c == ')') pcnt--;
			} while (pcnt > 0);
		}

		else return (*lptr++);
	}
}

struct table {
	char *name;
	int type, value;
};

static struct table mdtab[] = {
	{"January", MONTH, 1},
	{"February", MONTH, 2},
	{"March", MONTH, 3},
	{"April", MONTH, 4},
	{"May", MONTH, 5},
	{"June", MONTH, 6},
	{"July", MONTH, 7},
	{"August", MONTH, 8},
	{"September", MONTH, 9},
	{"Sept", MONTH, 9},
	{"October", MONTH, 10},
	{"November", MONTH, 11},
	{"December", MONTH, 12},

	{"Sunday", DAY, 0},
	{"Monday", DAY, 1},
	{"Tuesday", DAY, 2},
	{"Tues", DAY, 2},
	{"Wednesday", DAY, 3},
	{"Wednes", DAY, 3},
	{"Thursday", DAY, 4},
	{"Thur", DAY, 4},
	{"Thurs", DAY, 4},
	{"Friday", DAY, 5},
	{"Saturday", DAY, 6},
	{0, 0, 0}};

#define HRS *60
#define HALFHR 30
static struct table mztab[] = {
	{"a.m.", MERIDIAN, AM},
	{"am", MERIDIAN, AM},
	{"p.m.", MERIDIAN, PM},
	{"pm", MERIDIAN, PM},
	{"nst", ZONE, 3 HRS + HALFHR},		/* Newfoundland */
	{"n.s.t.", ZONE, 3 HRS + HALFHR},
	{"ast", ZONE, 4 HRS},		/* Atlantic */
	{"a.s.t.", ZONE, 4 HRS},
	{"adt", DAYZONE, 4 HRS},
	{"a.d.t.", DAYZONE, 4 HRS},
	{"est", ZONE, 5 HRS},		/* Eastern */
	{"e.s.t.", ZONE, 5 HRS},
	{"edt", DAYZONE, 5 HRS},
	{"e.d.t.", DAYZONE, 5 HRS},
	{"cst", ZONE, 6 HRS},		/* Central */
	{"c.s.t.", ZONE, 6 HRS},
	{"cdt", DAYZONE, 6 HRS},
	{"c.d.t.", DAYZONE, 6 HRS},
	{"mst", ZONE, 7 HRS},		/* Mountain */
	{"m.s.t.", ZONE, 7 HRS},
	{"mdt", DAYZONE, 7 HRS},
	{"m.d.t.", DAYZONE, 7 HRS},
	{"pst", ZONE, 8 HRS},		/* Pacific */
	{"p.s.t.", ZONE, 8 HRS},
	{"pdt", DAYZONE, 8 HRS},
	{"p.d.t.", DAYZONE, 8 HRS},
	{"yst", ZONE, 9 HRS},		/* Yukon */
	{"y.s.t.", ZONE, 9 HRS},
	{"ydt", DAYZONE, 9 HRS},
	{"y.d.t.", DAYZONE, 9 HRS},
	{"hst", ZONE, 10 HRS},		/* Hawaii */
	{"h.s.t.", ZONE, 10 HRS},
	{"hdt", DAYZONE, 10 HRS},
	{"h.d.t.", DAYZONE, 10 HRS},

	{"gmt", ZONE, 0 HRS},
	{"g.m.t.", ZONE, 0 HRS},
	{"ut", ZONE, 0 HRS},
	{"u.t.", ZONE, 0 HRS},
	{"bst", DAYZONE, 0 HRS},		/* British Summer Time */
	{"b.s.t.", DAYZONE, 0 HRS},
	{"eet", ZONE, 0 HRS},		/* European Eastern Time */
	{"e.e.t.", ZONE, 0 HRS},
	{"eest", DAYZONE, 0 HRS},	/* European Eastern Summer Time */
	{"e.e.s.t.", DAYZONE, 0 HRS},
	{"met", ZONE, -1 HRS},		/* Middle European Time */
	{"m.e.t.", ZONE, -1 HRS},
	{"mest", DAYZONE, -1 HRS},	/* Middle European Summer Time */
	{"m.e.s.t.", DAYZONE, -1 HRS},
	{"wet", ZONE, -2 HRS },		/* Western European Time */
	{"w.e.t.", ZONE, -2 HRS },
	{"west", DAYZONE, -2 HRS},	/* Western European Summer Time */
	{"w.e.s.t.", DAYZONE, -2 HRS},

	{"jst", ZONE, -9 HRS},		/* Japan Standard Time */
	{"j.s.t.", ZONE, -9 HRS},	/* Japan Standard Time */
					/* No daylight savings time */

	{"aest", ZONE, -10 HRS},	/* Australian Eastern Time */
	{"a.e.s.t.", ZONE, -10 HRS},
	{"aesst", DAYZONE, -10 HRS},	/* Australian Eastern Summer Time */
	{"a.e.s.s.t.", DAYZONE, -10 HRS},
	{"acst", ZONE, -(9 HRS + HALFHR)},	/* Australian Central Time */
	{"a.c.s.t.", ZONE, -(9 HRS + HALFHR)},
	{"acsst", DAYZONE, -(9 HRS + HALFHR)},	/* Australian Central Summer */
	{"a.c.s.s.t.", DAYZONE, -(9 HRS + HALFHR)},
	{"awst", ZONE, -8 HRS},		/* Australian Western Time */
	{"a.w.s.t.", ZONE, -8 HRS},	/* (no daylight time there, I'm told */
	{0, 0, 0}};

static struct table unittb[] = {
	{"year", MUNIT, 12},
	{"month", MUNIT, 1},
	{"fortnight", UNIT, 14*24*60},
	{"week", UNIT, 7*24*60},
	{"day", UNIT, 1*24*60},
	{"hour", UNIT, 60},
	{"minute", UNIT, 1},
	{"min", UNIT, 1},
	{"second", SUNIT, 1},
	{"sec", SUNIT, 1},
	{0, 0, 0}};

static struct table othertb[] = {
	{"tomorrow", UNIT, 1*24*60},
	{"yesterday", UNIT, -1*24*60},
	{"today", UNIT, 0},
	{"now", UNIT, 0},
	{"last", NUMBER, -1},
	{"this", UNIT, 0},
	{"next", NUMBER, 2},
	{"first", NUMBER, 1},
	/* {"second", NUMBER, 2}, */
	{"third", NUMBER, 3},
	{"fourth", NUMBER, 4},
	{"fifth", NUMBER, 5},
	{"sixth", NUMBER, 6},
	{"seventh", NUMBER, 7},
	{"eigth", NUMBER, 8},
	{"ninth", NUMBER, 9},
	{"tenth", NUMBER, 10},
	{"eleventh", NUMBER, 11},
	{"twelfth", NUMBER, 12},
	{"ago", AGO, 1},
	{0, 0, 0}};

static struct table milzone[] = {
	{"a", ZONE, 1 HRS},
	{"b", ZONE, 2 HRS},
	{"c", ZONE, 3 HRS},
	{"d", ZONE, 4 HRS},
	{"e", ZONE, 5 HRS},
	{"f", ZONE, 6 HRS},
	{"g", ZONE, 7 HRS},
	{"h", ZONE, 8 HRS},
	{"i", ZONE, 9 HRS},
	{"k", ZONE, 10 HRS},
	{"l", ZONE, 11 HRS},
	{"m", ZONE, 12 HRS},
	{"n", ZONE, -1 HRS},
	{"o", ZONE, -2 HRS},
	{"p", ZONE, -3 HRS},
	{"q", ZONE, -4 HRS},
	{"r", ZONE, -5 HRS},
	{"s", ZONE, -6 HRS},
	{"t", ZONE, -7 HRS},
	{"u", ZONE, -8 HRS},
	{"v", ZONE, -9 HRS},
	{"w", ZONE, -10 HRS},
	{"x", ZONE, -11 HRS},
	{"y", ZONE, -12 HRS},
	{"z", ZONE, 0 HRS},
	{0, 0, 0}};

static
lookup(id)
char *id;
{
#define gotit (yylval=i->value,  i->type)
#define getid for(j=idvar, k=id; *j++ = *k++; )

	char idvar[20];
	register char *j, *k;
	register struct table *i;
	int abbrev;

	getid;
	if (strlen(idvar) == 3) abbrev = 1;
	else if (strlen(idvar) == 4 && idvar[3] == '.') {
		abbrev = 1;
		idvar[3] = '\0';
	}
	else abbrev = 0;

	if (islower(*idvar)) *idvar = toupper(*idvar);

	for (i = mdtab; i->name; i++) {
		k = idvar;
		for (j = i->name; *j++ == *k++;) {
			if (abbrev && j==i->name+3) return gotit;
			if (j[-1] == 0) return gotit;
		}
	}

	getid;
	for (i = mztab; i->name; i++)
		if (strcmp(i->name, idvar) == 0) return gotit;

	for (j = idvar; *j; j++)
		if (isupper(*j)) *j = tolower(*j);
	for (i=mztab; i->name; i++)
		if (strcmp(i->name, idvar) == 0) return gotit;

	getid;
	for (i=unittb; i->name; i++)
		if (strcmp(i->name, idvar) == 0) return gotit;

	if (idvar[strlen(idvar)-1] == 's')
		idvar[strlen(idvar)-1] = '\0';
	for (i=unittb; i->name; i++)
		if (strcmp(i->name, idvar) == 0) return gotit;

	getid;
	for (i = othertb; i->name; i++)
		if (strcmp(i->name, idvar) == 0) return gotit;

	getid;
	if (strlen(idvar) == 1 && isalpha(*idvar)) {
		if (isupper(*idvar)) *idvar = tolower(*idvar);
		for (i = milzone; i->name; i++)
			if (strcmp(i->name, idvar) == 0) return gotit;
	}

	return(ID);
}

time_t
parse(p, now)
char *p;
struct timeb *now;
{
#define mcheck(f)	if (f>1) err++
	time_t monthadd();
	int err;
	struct tm *lt;
	struct timeb ftz;

	time_t sdate, tod;

	lptr = p;
	if (now == ((struct timeb *) NULL)) {
		now = &ftz;
		ftime(&ftz);
	}
	lt = localtime(&now->time);
	year = lt->tm_year;
	month = lt->tm_mon+1;
	day = lt->tm_mday;
	relsec = 0; relmonth = 0;
	timeflag=zoneflag=dateflag=dayflag=relflag=0;
	ourzone = now->timezone;
	daylite = MAYBE;
	hh = mm = ss = 0;
	merid = 24;

	if (err = yyparse()) return (-1);

	mcheck(timeflag);
	mcheck(zoneflag);
	mcheck(dateflag);
	mcheck(dayflag);

	if (err) return (-1);
	if (dateflag || timeflag || dayflag) {
		sdate = dateconv(month,day,year,hh,mm,ss,merid,ourzone,daylite);
		if (sdate < 0) return -1;
	}
	else {
		sdate = now->time;
		if (relflag == 0)
			sdate -= (lt->tm_sec + lt->tm_min*60 +
				lt->tm_hour*(60L*60L));
	}

	sdate += relsec;
	sdate += monthadd(sdate, relmonth);

	if (dayflag && !dateflag) {
		tod = dayconv(dayord, dayreq, sdate);
		sdate += tod;
	}

	return sdate;
}

yyerror(s) char *s;
{}
