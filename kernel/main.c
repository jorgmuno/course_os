#include "include/mmap.h"
#include "include/pmap.h"
#include "include/vmlayout.h"
#include "include/klibc.h"

void main(void){
  print_vuart0("New supervisor stack at 0xefffd000\n");

  //Confirm new stack by push test value
  asm volatile(
    "ldr r0, =0x313 \n\t"
    "push {r0}");   

  //flush TLB
  asm volatile(
    "eor r0, r0 \n\t"
    "MCR p15, 0, r0, c8, c7, 0 \n\t");

  //Unmap one-to-one kernel and pt mappings
  *(v_first_level_pt+KERNSTART+(KERNDSBASE>>20)) = 0;   
  *(v_first_level_pt+KERNSTART) = 0;

/*
  int i;
  for(i=0;i<4096;i++){
    v_printf("i= %d\n",i);   
    v_printf("l1pt addr= %x\t", (first_level_pt+i));  
    v_printf("l1pte= %x\n", first_level_pt[i]);  
    v_printf("l1vpt addr= %x\t", (v_first_level_pt+i));  
    v_printf("l1vpte= %x\n", *(v_first_level_pt+i));  
  }
*/
  v_printf("Ready for malloc\n");
  unsigned int * test = (unsigned int*)u_malloc(sizeof(*test));
  v_printf("%x\n", test);
  *test = 0x777;

  struct s
  {
    int x;
    int *y;
    char z;
  };

  struct s* stest = (struct s*)k_malloc(sizeof(*stest));
  v_printf("%x\n", stest);
  stest->x = 0x333;
  stest->y = (int*)0x3333333;
  stest->z = 'a';

  asm volatile("wfi");

}