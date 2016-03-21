#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int readGadgetBinary(char *filename)
{
  
  int i, j, dummy, nread=0;
  int N_tot, N_min, N_max;
  float faux[3], Maux;
  char label[4];
  unsigned int uintaux;
  FILE *fdata;
  
  ////////////////////////////////////////////////////////////////////////
  //        READ DATA FILE
  ////////////////////////////////////////////////////////////////////////
  
  
  fdata = fopen(filename,"r");
  
  printf("Reading snapshot %s\n",filename); 
  
  if (fdata == NULL) 
    printf("No se pudo abrir %s\n",filename);
  
  nread=fread(&dummy,sizeof(dummy),1,fdata);
  nread=fread(&label,sizeof(char),4,fdata);
  nread=fread(&dummy,sizeof(dummy),1,fdata);
  nread=fread(&dummy,sizeof(dummy),1,fdata);
  printf("%s\n",label);
  
  nread=fread(&dummy,sizeof(dummy),1,fdata);
  nread=fread(&Header,sizeof(struct io_header),1,fdata);
  nread=fread(&dummy,sizeof(dummy),1,fdata);
  
  NtotPart=0;
  printf("Reading snapshot with:\n");
  for(i=0; i<6; i++) 
    {
      NtotPart += Header.npartTotal[i];
      printf("Type %d has Npart=%12d NpartTotal=%12d with mass %16.8lf\n", i, 
	     Header.Npart[i], Header.npartTotal[i], Header.mass[i]);
    }
  
  printf("There is a total %d particles in the snapshot\n\n",NtotPart);
  printf(" * Frame's Time... %16.8f\n",Header.time);
  printf(" * Redshift... %16.8f\n",Header.redshift);
  printf(" * Flagsfr... %d\n",Header.flag_sfr);
  printf(" * Flagfed... %d\n",Header.flag_feedback);
  printf(" * Flagcool... %d\n",Header.flag_cooling);
  printf(" * numfiles... %d\n",Header.num_files);
  printf(" * Boxsize... %16.8f\n",Header.BoxSize);
  printf(" * Omega0... %16.8f\n",Header.Omega0);
  printf(" * OmageLa... %16.8f\n",Header.OmegaLambda);
  printf(" * Hubbleparam... %16.8f\n",Header.HubbleParam);  
  
  
  //------------------------------------------------------------------ 
  
  part = (struct particles *)malloc((size_t) NtotPart*sizeof(struct particles));  
  
  if(part == NULL)
    {
      printf("No se pudo alocar particles\n");
      exit(0);
    }
  
  
  //-----------------------------------------------POSITIONS
  
  nread=fread(&dummy,sizeof(dummy),1,fdata);
  nread=fread(&label,sizeof(char),4,fdata);
  nread=fread(&dummy,sizeof(dummy),1,fdata);
  nread=fread(&dummy,sizeof(dummy),1,fdata);
  printf("%s\n",label);
  
  nread=fread(&dummy,sizeof(dummy),1,fdata);
  for(i=0; i<NtotPart; i++) 
    {
      nread=fread(&faux[0], sizeof(float), 3, fdata); 
      part[i].pos[0] = faux[0];
      part[i].pos[1] = faux[1];
      part[i].pos[2] = faux[2];

      if(i<10)
	printf("%16.8f %16.8f %16.8f\n",part[i].pos[0], part[i].pos[1], part[i].pos[2]);
    }
  nread=fread(&dummy,sizeof(dummy),1,fdata);
  
  if(dummy != (3*NtotPart*sizeof(float)))
    {
      printf(" Can not read properly ids %d %lu\n",dummy,3*NtotPart*sizeof(float));
      exit(0);
    }
  
  
  //-------------------------------------------------------------VELOCITIES
  
  nread=fread(&dummy,sizeof(dummy),1,fdata);
  nread=fread(&label,sizeof(char),4,fdata);
  nread=fread(&dummy,sizeof(dummy),1,fdata);
  nread=fread(&dummy,sizeof(dummy),1,fdata);
  printf("%s\n",label);
  

  nread=fread(&dummy,sizeof(dummy),1,fdata);
  for(i=0; i<NtotPart; i++)
    {
      nread=fread(&faux[0],sizeof(float),3,fdata);
      part[i].vel[0] = faux[0];
      part[i].vel[1] = faux[1];
      part[i].vel[2] = faux[2];

      if(i<10)
	printf("%16.8f %16.8f %16.8f\n",part[i].vel[0], part[i].vel[1], part[i].vel[2]);
    }
  nread=fread(&dummy,sizeof(dummy),1,fdata);
  
  if(dummy != (3*NtotPart*sizeof(float)))
    {
      printf("Can not read properly ids %d %lu\n",dummy,3*NtotPart*sizeof(float));
      exit(0);
    }
  

  
  //---------------------------------------------------IDS
  
  nread=fread(&dummy,sizeof(dummy),1,fdata);
  nread=fread(&label,sizeof(char),4,fdata);
  nread=fread(&dummy,sizeof(dummy),1,fdata);
  nread=fread(&dummy,sizeof(dummy),1,fdata);
  printf("%s\n",label);
  

  nread=fread(&dummy,sizeof(dummy),1,fdata);
  
  for(i=0; i<NtotPart; i++)
    {
      nread=fread(&uintaux,sizeof(unsigned int),1,fdata); 
      part[i].id = uintaux;
    }
  nread=fread(&dummy,sizeof(dummy),1,fdata);
  
  if(dummy != (NtotPart*sizeof(unsigned int)))
    {
      printf(" Can not read properly ids %d %lu\n",dummy,NtotPart*sizeof(unsigned int));
      exit(0);
    }
  
  
  
  //-----------------------------------------MASSES
  
  nread=fread(&dummy,sizeof(dummy),1,fdata);
  nread=fread(&label,sizeof(char),4,fdata);
  nread=fread(&dummy,sizeof(dummy),1,fdata);
  nread=fread(&dummy,sizeof(dummy),1,fdata);
  printf("%s\n",label);
  

  nread=fread(&dummy,sizeof(dummy),1,fdata);
  
  N_min = N_max=0;
  
  for(j=0; j<=5; j++)
    {
      N_max=N_max+Header.npartTotal[j];
      if((Header.mass[j]==0) && (Header.npartTotal[j]!=0)) 
	{
	  for(i=N_min;i<N_max;i++)
	    {
	      nread=fread(&Maux,sizeof(float),1,fdata);  
	      part[i].mass = Maux;
	    }
	}
      
      if((Header.mass[j]!=0) && (Header.npartTotal[j]!=0)) 
	{
	  for(i=N_min;i<N_max;i++)
	    {
	      part[i].mass = Header.mass[j];
	    }
	}
      N_min=N_max; 
    }
  
  nread=fread(&dummy,sizeof(dummy),1,fdata);
  
  
  
  return 0;
 
}
