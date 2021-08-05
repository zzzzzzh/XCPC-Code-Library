// 据说会快 orz
inline int multmod(int x,int y, int MOD) {
  int ret;
  __asm__ __volatile__ ("\tmull %%ebx\n\tdivl %%ecx\n":"=d"(ret):"a"(x),"b"(y),"c"(MOD));
  return ret;
}

