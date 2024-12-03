//=========BMPIO======== - BITMAP IMAGE I/O HEADER LIBRARY - ========C,C++ COMPATIBILITY========
//DEVELOPED BY NGUYEN THANH LAN
//EMAIL: lantramnamcodon@gmail.com

struct BMPIO{
bool mode=0;
bool format=0;
int dpi=2835;
}bmpio;

void bmpio_set_dpi(int value){
bmpio.dpi=value;
}

void bmpio_load_from(const char* source){
if(source=="FILE"){
bmpio.mode=0;
}else if(source=="PIPELINE"){
bmpio.mode=1;
}
}

void bmpio_load_as(const char* format){
if(format=="RGB"){
bmpio.format=0;
}else if(format=="RGBA"){
bmpio.format=1;
}
}

unsigned char* bmpio_loader(const char* filename,int *imgwi,int *imghe,int *bitdepth){
FILE *bitmapfile;
unsigned char header[54],*imagedata,*bitmapdata;
unsigned int *ip,filesize,rastersize,byteoffset;
if(!bmpio.mode){//read from file
bitmapfile=fopen(filename,"rb");
fread(header,1,54,bitmapfile);
fclose(bitmapfile);
}else{//read from pipeline
bitmapfile=popen(filename,"rb");
fread(header,1,54,bitmapfile);
pclose(bitmapfile);
}
if(header[0]=='B'&&header[1]=='M'){ //check if the file is truly bitmap
ip=(unsigned int*)&header[2];
filesize=*ip; //obtain file size
ip=(unsigned int*)&header[10];
byteoffset=*ip; //byte offset to pixel
ip=(unsigned int*)&header[18];
*imgwi=*ip; //obtain image width
ip=(unsigned int*)&header[22];
*imghe=*ip; //obtain image height
short *sp=(short*)&header[28];
*bitdepth=*sp; //obtain bitmap type
rastersize=filesize-byteoffset; //obtain pixel data size
bitmapdata=(unsigned char*)calloc(filesize,1);
if(!bmpio.mode){
bitmapfile=fopen(filename,"rb");
fread(bitmapdata,1,filesize,bitmapfile);
fclose(bitmapfile);
}else{
bitmapfile=popen(filename,"rb");
fread(bitmapdata,1,filesize,bitmapfile);
pclose(bitmapfile);
}
bitmapdata+=byteoffset;
//prepare correct data
if(!bmpio.format){
imagedata=(unsigned char*)calloc(rastersize,1);
for(int i=0;i<rastersize;i+=3){
imagedata[i]=bitmapdata[i+2];
imagedata[i+1]=bitmapdata[i+1];
imagedata[i+2]=bitmapdata[i];
}
}else{
imagedata=(unsigned char*)calloc(rastersize/3+rastersize,1);
int index=0;
for(int i=0;i<rastersize;i+=3){
imagedata[index]=bitmapdata[i+2];
imagedata[index+1]=bitmapdata[i+1];
imagedata[index+2]=bitmapdata[i];
imagedata[index+3]=255;
index+=4;
}
}
free(bitmapdata-byteoffset);
return imagedata;
}else{
return NULL;
}
}

void bmpio_writer(const char *filename,int imgwi,int imghe,int format,unsigned char *imagedata){
unsigned int *ip,filesize,datasize;
unsigned char *filedata;
short *sp;
if(format>=3){
datasize=imgwi*imghe*3;
filesize=datasize+54;
filedata=(unsigned char*)calloc(filesize,1);

//compose header
filedata[0]='B';
filedata[1]='M';
ip=(unsigned int*)&filedata[2];
*ip=filesize;
ip=(unsigned int*)&filedata[10];
*ip=54;
ip=(unsigned int*)&filedata[14];
*ip=40;
ip=(unsigned int*)&filedata[18];
*ip=imgwi;
ip=(unsigned int*)&filedata[22];
*ip=imghe;
sp=(short*)&filedata[26];
*sp=1;
sp=(short*)&filedata[28];
*sp=24;
ip=(unsigned int*)&filedata[38];
*ip=bmpio.dpi;
ip=(unsigned int*)&filedata[42];
*ip=(float)imghe/(float)imgwi*(float)bmpio.dpi;
filedata+=54;
//write image data
int index=0;
for(int i=0;i<datasize;i+=3){
filedata[i]=imagedata[index+2];
filedata[i+1]=imagedata[index+1];
filedata[i+2]=imagedata[index];
index+=format;
}
filedata-=54;
//export data into file
FILE *outputfile=fopen(filename,"wb");
fwrite(filedata,1,filesize,outputfile);
fclose(outputfile);
free(filedata);
}
}
