static char sccs_id[] = "@(#)em.c	20.7 3/11/98";
/*
*           IPF V2.x: Interactive PowerFlow Version 2.x
*              Bonneville Power Administration (BPA)
*         Authors:  D. L. Clark, J. G. Coleman, W. L. Powell, 
*                B. E. Rogers, K. E. Rowell, J. L. Rutis, D. M. Stefonik, 
*                D. M. Syzmanski 
*
*                              NOTICE
*
* Interactive Powerflow (IPF) was developed by BPA and its contractors
* with about 20% of the cost supported by the Electric Power Research
* Institute (EPRI).  By mutual agreement, as described in EPRI 
* Agreement RP2746-03 entitled Graphical User Interface for 
* Powerflow, March, 1992, all results of this project--including 
* the computer program and its documentation--are to be in the 
* public domain.  In a separate Memorandum of Understanding with the 
* Western Systems Coordinating Council (WSCC), BPA agreed in March, 1992, 
* to keep WSCC informed of progress, to make its best effort to develop 
* the program according to the Guidelines adopted by the WSCC Computer 
* Program Management Subcommittee, and to make the final results 
* available for possible further development by WSCC. 
*
* This notice must appear in all copies of this software and documentation.
*/
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <Xm/Xm.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include "dmgr.h"
#include "fm.h"
#include "em.h"

extern void ipf_error	(char *msg, char *id, char *lnk, int line, int dlg );
extern void fetch_widget(char *wname);
extern Widget widget_id ( char * );
extern void find_link   ( char *link );
extern void initwin     ( );
extern void ipf_itoa    ( int, char * );
extern void Vuit_MU     ( char *, char * );
extern int HashLookup   ( char *name, Widget *id);

#define LOG_ERRORS_ASC 0 /* defined=do compile code to print em_log file */
static char help_link[100];


void em_show(emERROR *err)
/****************************************************************************\
*  standard error message to error dialog box and "em_log" file.
*
*  author: D.Szymanski		Modifications: 	B.Rogers  9/16/93
*
*  called by: too many to list!
\****************************************************************************/
{

#ifdef LOG_ERRORS_ASC
  FILE *fp;
  int stat;
  time_t tp=time(NULL);

  if ((fp = gopen("em_log","a+")) == NULL) {
    perror("Routine em.c cannot open em_log file"); }
#endif

    /* force the warning dialog to pop up */
  ipf_error(err->msg, err->ident, err->link, err->line, 1);	/* errdlg.c */

#ifdef LOG_ERRORS_ASC
  if ((stat = fprintf(fp,"FILE=%s, LINE=%d, TYPE=%d, DATE=%24.24s\
 LINK=%s, IDENT=%s, MSG={%s}\n",
      err->file, err->line, err->type, ctime(&tp), err->link, err->ident,
      err->msg))==EOF)
  {
    perror("Routine em.c cannot write to em_log");
  }

  if ((stat = fclose(fp)) == EOF) {
    perror("Routine em.c cannot close em_log file");
  }

#else

	/* Abnormal style: Print to screen instead of em_log file */
  printf("%78.78s\n", err->msg);

/*  OLD WAY */
/*  printf("FILE=%s, LINE=%d, TYPE=%d, LINK=%s, IDENT=%s\n\ MSG=%s\n",	 */
/*    err->file, err->line, err->type, err->link, err->ident, err->msg); */

#endif
}


/*****************************************************************************\
*       Function: errdlg.c
*               position error dialog box to last msg.
*
*       Author: dbs              Modified: Sept 22, 1992
*
*
\*****************************************************************************/

void ipf_error(char *error_message,
               char *identifier,
               char *link,
               int line_number,
               int manage_dialog)
{
  Widget id;
  char *c;
  char  line_no_str[6];
  static Boolean  err_msg_dlg_not_chkd = True;

  ipf_itoa(line_number, line_no_str );
  line_no_str[5] = '\0';

  strncpy( help_link, link, 100 );

  if( err_msg_dlg_not_chkd )
  {
    fetch_widget("error_message_dialog");
    if((HashLookup("error_message_dialog", &id)) == False)
    {
      printf("***ERROR*** in ipf_error (errdlg.c) *PROGRAM ERROR*\n" );
      printf("       error_message_dialog window not available, register or managed.\n" );
      printf("       Attempt to display missing window in it's own self\n");
      printf("       will result in an infinitive loop!\n" );
    }
    err_msg_dlg_not_chkd = False;
  }
  else
  {
  if(manage_dialog)
    Vuit_MU( "M",  "error_message_dialog" );

  id = widget_id( "error_dia_message_text" );
  if(id == NULL) return;

  XmTextSetTopCharacter(id, XmTextGetLastPosition(id));

  XmTextInsert ( id,
                 XmTextGetLastPosition(id),
                 error_message );

  id = widget_id( "error_dia_identifier_text" );
  if(id == NULL) return;

/* Remove any Horizontal TABS.... alpha system can't tolerate them! */
  c = identifier;
  while ( *c )
  {
    if( *c == 9 )
    {
       *c = ' ';
    }
    c++;
  }

  XmTextFieldSetString( id, identifier );
  id = widget_id( "error_dia_line_text" );
  if(id == NULL) return;
  XmTextFieldSetString( id, line_no_str );
  }

}

error_dia_help_cb(w, tag, reason)
Widget  w;
XtPointer tag;
XtPointer reason;
{
  Vuit_MU( "M", "help_dialog");
  initwin();
  find_link(help_link);
}

