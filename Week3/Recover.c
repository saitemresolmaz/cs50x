#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[])
{
if (argc != 2)
{
fprintf(stderr, "Usage: ./recover card.raw\n");
return 1;
}
// hafıza kartını aç
FILE* inptr = fopen("card.raw", "r");
if (inptr == NULL)
{
fclose(inptr);
fprintf(stderr, "Could not open card.raw");
return 1;
}
// 512 byte'lık bir geçici yer tanımladık.
unsigned char buffer[512];
// How many jpgs found
int counter = 0;
// Current filename and img
char title[10];
FILE* img;
// Read until end of card
while(fread(&buffer, 512, 1, inptr)) //512 =size 1 = count
{
// start of new jpg?
if (buffer[0] == 0xff && buffer[1] == 0xd8 &&
buffer[2] == 0xff && buffer[3] >> 4 == 0xe) //>> 4 bit sağa kaydırınca = e ise
{
// Found jpg
counter++;
// Close prev file (if any)
if (counter > 1)
{
fclose(img);
}
// Open new file and write first buffer
sprintf(title, "%03d.jpg", counter - 1); // toplamda 3 digit. Sonuncusu ama 0 olacak.
img = fopen(title, "a");
}
// If currently writing to file, write buffer
if (counter > 0)
{
fwrite(&buffer, 512, 1, img);
}
}
// close any remaining files
fclose(img);
fclose(inptr);
}