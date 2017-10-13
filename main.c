/*
Main program for the virtual memory project.
Make all of your modifications to this file.
You may add or rearrange any code or data as you need.
The header files page_table.h and disk.h explain
how to use the page table and disk interfaces.
*/

#include "page_table.h"
#include "disk.h"
#include "program.h"
#include "time.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define getName(var)  #var
struct disk* disk;
int faults;
int* fframes; // arreglo de marcos libres
int* faults_per_page;
int* mapped_pages; //paginas en memoria actualmente 
int fframes_len; // total de marcos libres, las paginas cargadas en memoria seran nframes-fframes_len

void print_narray(int* array,int len);

void mapped_pages_append(int mp_len, int page){ //  se agregan como si fuese una cola fifo, 
								//eso nos sirve para cualquier modo, en eliminar cambia la logica
								//mapped_pages_len=nframes-fframes_len
								
	if(mp_len==0){
		mapped_pages=malloc(sizeof(int)*(mp_len+1));
	
	}			
	else{	
		mapped_pages=realloc(mapped_pages,sizeof(int)*(mp_len+1));
		
		
		}
	if(mapped_pages==NULL){
		fprintf(stderr,"realloc error");
		exit(1);
	
	}
	*(mapped_pages+mp_len)=page;
	
}

void mapped_pages_delete(int mp_len,int page){
	int index=mp_len-1;
	for(int i=0;i<mp_len-1;i++){
	
		if(mapped_pages[i]==page){
			index=i;
		
		}
		
		if(i>=index){
		
			mapped_pages[i]=mapped_pages[i+1];
		}
		
		
	
	}
	
	mapped_pages=realloc(mapped_pages,sizeof(int)*(mp_len-1));
	
	

}




void fframes_append(int frame){ //agrega el frame a 'fframes'.. no aumenta el contador

	fframes=realloc(fframes,sizeof(int)*(fframes_len+1));
	fframes[fframes_len]=frame;
	return;


}

void fframes_delete(int frame){
	int index=fframes_len-1;
	for(int i=0;i<fframes_len-1;i++){
		if(fframes[i]==frame){
			index=i;
		}
		if(i>=index){
			fframes[i]=fframes[i+1];
		
		}
		
	
	}
	
	fframes=realloc(fframes,sizeof(int)*(fframes_len-1));

}



int get_vpage(char mode){ // mode='f', mode='r', mode... etc (fifo,random,etc)
	
	if(mode=='f'){
		return mapped_pages[0];	
	}
	//rellenar con los else if correspondientes segun algoritmo de reemplazo usado

}
void fifo_pfh( struct page_table *pt, int page )
{	

	int nframes=page_table_get_nframes(pt);
	int mp_len=nframes-fframes_len;
	printf("mapped pages:%d\n",mp_len);
	printf("fframes_len:%d\n",fframes_len);
	printf("page fault on page #%d\n",page);
	faults+=1;
	int vframe; //marco victima
	int bits;
	//nos fijamos si existe algun marco libre para asignar, claramente la pagina no esta en memoria
	if(fframes_len>0){   //si esque existe algun marco vacio
		vframe=fframes[0];
		char* frame_mem=(page_table_get_physmem(pt))+(PAGE_SIZE*vframe);
		disk_read(disk,page,frame_mem);
		page_table_set_entry(pt,page,vframe,PROT_WRITE);
		fframes_delete(vframe);
		mapped_pages_append(mp_len,page);
		fframes_len-=1;
	}
	else{
	
		//solicitamos pagina que sera sacada de memoria...
		int vpage=get_vpage('f');  // modo fifo
		
		//conseguimos el frame al cual esta mapeada..
		
		page_table_get_entry(pt,vpage,&vframe,&bits);
		
		//generamos la direccion fisica del frame para guardarla en disco...
		char* vframe_mem=(page_table_get_physmem(pt))+PAGE_SIZE*vframe;
		
		//escribimos efectivamente el frame completo (o la pagina victima) al disco
		
		disk_write(disk,vpage,vframe_mem);
		
		//leemos desde el disco el bloque de la nueva pagina para cargarla al marco victima 'vframe'..
		
		disk_read(disk,page,vframe_mem);
		
		//actualizamos la entrada de la pagina que pasa al disco
		
		page_table_set_entry(pt,vpage,0,0);
		
		//actualizamos la entrada de la pagina que paso del disco a memoria
		
		page_table_set_entry(pt,page,vframe,PROT_WRITE);
		
		//eliminamos de 'mapped_frames' la pagina que paso al disco
		
		mapped_pages_delete(mp_len,vpage);
		mp_len-=1;
		
		//agregamos a 'mapped_frames' la pagina que pasa a memoria
		mapped_pages_append(mp_len,page);
		mp_len+=1;
		
		
		//no modificamos 'fframes' porque en realidad el marco se desocupo pero al mismo tiempo fue usado por la nueva pagina
	
	}
	
		
	//	faults_per_page[page]+=1;
	//primero ver si el fault es porque la pagina no esta en memoria
	
	
	
	
	//exit(1);
	return;
}

int main( int argc, char *argv[] )
{	

	printf("bits de lectura:'%d'\n",PROT_READ);
	printf("bits de escritura:'%d'\n",PROT_WRITE);
	printf("bits de exec:'%d'\n",PROT_EXEC);
	
	if(argc!=5) {
		/* Add 'random' replacement algorithm if the size of your group is 3 */
		printf("use: virtmem <npages> <nframes> <lru|fifo> <sort|scan|focus>\n");
		return 1;
	}
	faults=0;
	int npages = atoi(argv[1]);
	int nframes = atoi(argv[2]);
	char* alg=argv[3];
	
	
	if(!strcmp(alg,"fifo")){
		printf("algoritmo fifo\n");
	
	}
	
	else if(!strcmp(alg,"custom") || !strcmp(alg,"cust")){
	
		printf("algoritmo custom\n");
		
		//inicializamos nuestro arreglo con ceros
		faults_per_page=malloc(sizeof(int)*npages);
		for(int i=0;i<npages;i++){
			faults_per_page[i]=0;
		
		}
		
	}
	
	
	else if(!strcmp(alg,"random") || !strcmp(alg,"rand")){
	
		printf("algoritmo random no implementado");
		return 0;
	
	}
	
	
	else{
	
		fprintf(stderr,"algoritmo no conocido");
		exit(1);
	}
	const char *program = argv[4];

	disk = disk_open("myvirtualdisk",npages);
	if(!disk) {
		fprintf(stderr,"couldn't create virtual disk: %s\n",strerror(errno));
		return 1;
	}

	struct page_table *pt = page_table_create( npages, nframes, fifo_pfh);
	if(!pt) {
		fprintf(stderr,"couldn't create page table: %s\n",strerror(errno));
		return 1;
	}

	char *virtmem = page_table_get_virtmem(pt);

	char *physmem = page_table_get_physmem(pt);
	
	// inicializamos 'fframes' nuestro arreglo de marcos libres
	fframes=malloc(nframes*sizeof(int));
	fframes_len=nframes;
	for (int i=0;i<nframes;i++){
		fframes[i]=i;
	}
	//
	
	
	if(!strcmp(program,"sort")) {
		sort_program(virtmem,npages*PAGE_SIZE);

	} else if(!strcmp(program,"scan")) {
		scan_program(virtmem,npages*PAGE_SIZE);

	} else if(!strcmp(program,"focus")) {
		focus_program(virtmem,npages*PAGE_SIZE);

	} else {
		fprintf(stderr,"unknown program: %s\n",argv[3]);

	}
	
	
	for(int i=0;i<fframes_len;i++){
	
		printf("fframes[%2d]: %d\n",i,fframes[i]);
	
	}
	
	
	
	

	
	
	
	printf("]");
	printf("\n");
	
	
	for(int i=0;i<fframes_len;i++){
	
		printf("fframes[%2d]: %d\n",i,fframes[i]);
		
	}
	
	printf("-----------FAULTS SUMMARY: %d PAGE FAULTS------------\n",faults);
	free(faults_per_page);
	free(mapped_pages);
	page_table_delete(pt);
	disk_close(disk);
	return 0;
}
