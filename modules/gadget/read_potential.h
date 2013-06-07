#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <Python.h>
#include <numpy/arrayobject.h>

/*##################### Potential ############################*/
void gadget_readpotential()
{
  float *simdata;
  int ndim = 1;

  int i;
  unsigned int n;
  unsigned int pc = 0;

  unsigned int skip1,skip2;
  char* blocklabel = "POTENTIAL";

  for(j=0;j<NumFiles;j++){
    skip_blocks(values);
    if(header.flag_potential==0)
      PyErr_Format(PyExc_IndexError,"flag_potential=%d --> potentials not output",header.flag_potential);
    
    if(j==0){
      npy_intp dims[1]={header.npartTotal[type]};
      array = (PyArrayObject *)PyArray_SimpleNew(ndim,dims,PyArray_DOUBLE);
    }
    
    simdata=(float*)malloc(header.npart[type]*sizeof(float));
    
    fread(&skip1,sizeof(int),1,infp);
    for(i=1;i<type;i++)
      fseek(infp,header.npart[i-1]*sizeof(float),SEEK_CUR);
    fread(simdata,header.npart[type]*sizeof(float),1,infp);
    fread(&skip2,sizeof(int),1,infp);
    errorcheck(skip1,skip2,blocklabel);
    fclose(infp);
    
    for(n=0;n<header.npart[type];n++)
      {
	MDATA(array,pc) = simdata[n];
	pc++;
      }
  }
  if(pc!=header.npartTotal[type])
      PyErr_Format(PyExc_IndexError,"particle count mismatch!");
  return;
}

