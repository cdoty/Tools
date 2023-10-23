
/* Return a subset of data from an input file */

/* Compiled with DJGPP */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <conio.h>

void main(int argc, char **argv)
   {
   FILE *fpi, *fpo;
   char c1,c2;
   char fname[20];
   long start = 0;
   long length = 0;
   long datacount = 0;
   long position = 0;
   char *stopstring;

   /* Get start position from command line */

   if ( argc > 1)
      {
      if (argv[1][0] == '$')
         {
         start = strtol(argv[1]+1, &stopstring, 16);
         }
      else
         {
         start = strtol(argv[1], &stopstring, 10);
         }

      printf("Start position = %ld ($%lx)", start, start);
      }

   /* Get length from command line */

   if ( argc > 2)
      {
      if (argv[2][0] == '$')
         {
         length = strtol(argv[2]+1, &stopstring, 16);
         }
      else
         {
         length = strtol(argv[2], &stopstring, 10);
         }
      if (length == 0)
         {
         printf(": Length = 0 (Remainder of input file)");
         }
      else
         {
         printf(": Length = %ld ($%lx)", length, length);
         }
      }

   printf("\n");

   if ( argc > 4)
      {
      strcpy(fname, argv[3]);
      }
   else
      {
      printf("** subset v1.0 **, by Jeff F., 99-Jul-30\n");
      printf("  - Return a range of data from an input file.\n");
      printf("\n");
      printf("Usage: subset start length infile outfile\n\n");
      printf("       start = number (0 = first byte of input file)\n");
      printf("       length = number (0 = remainder of input file)\n");
      printf("\n");
      printf("       If number starts with $ it's hex, otherwise decimal.\n");

      exit(1);
      }

   if ((fpi = fopen(fname, "rb")) == NULL)
      {
      printf("Error opening file '%s'\n", fname);
      exit(1);
      }
   else
      printf("Reading from file '%s'\n", fname);

   /*If an output file name was specified then get it */
   strcpy(fname, argv[4]);

   printf("Writing to file '%s'\n", fname);

   fpo = fopen(fname, "wb");

   /* Skip over any leading unwanted bytes */

   while ( (feof(fpi) == 0) &&
           (position != start) )
      {
      c1 = fgetc(fpi);
      position++;
      }

   /* Copy the desired data */

   c1 = fgetc(fpi);

   while ( (feof(fpi) == 0) &&
           ((datacount < length) || (length == 0)) )
      {
      c2 = fgetc(fpi);
      fputc(c1,fpo);
      datacount++;
      c1 = c2;
      }

   fclose(fpi);
   fclose(fpo);

   exit(0);
   }