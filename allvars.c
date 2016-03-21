
//read a binary file. we must past the snapshot (ejm: cosmo_050) by the command line when running.

struct io_header
{
  int      Npart[6];
  double   mass[6];
  double   time;
  double   redshift;
  int      flag_sfr;
  int      flag_feedback;
  int      npartTotal[6];
  int      flag_cooling;
  int      num_files;
  double   BoxSize;
  double   Omega0;
  double   OmegaLambda;
  double   HubbleParam; 
  char     fill[256- 6*4- 6*8- 2*8- 2*4- 6*4- 2*4 - 4*8];  /* fills to 
							      256 Bytes */
};

struct particles
{
  float pos[3];
  float vel[3];
  int id;
  double mass;
  float v;
};


struct cells
{
  
  /* coordinates of the center of the cell  */
  float x;
  float y;
  float z;
  float rho;
  int cell_ID;
  int Np;
  double sigmaV;
  /*
    
  */
};


int NtotPart;
struct particles *part=NULL;
struct io_header Header;
