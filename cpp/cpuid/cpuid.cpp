
typedef unsigned long DWORD;

class CPUID
{
 public:
  CPUID() {}
  void Executecpuid(DWORD eax); // 用来实现CPUID
 private:
  DWORD m_eax;   // 存储返回的eax
  DWORD m_ebx;   // 存储返回的ebx
  DWORD m_ecx;   // 存储返回的ecx
  DWORD m_edx;   // 存储返回的edx
};

 void CPUID::Executecpuid(DWORD veax)
 {
  DWORD deax;
  DWORD debx;
  DWORD decx;
  DWORD dedx;

  __asm
  {
   mov eax, veax
   cpuid  
   mov deax, eax
   mov debx, ebx
   mov decx, ecx
   mov dedx, edx
  }

  m_eax = deax; // 把临时变量中的内容放入类成员变量
  m_ebx = debx;
  m_ecx = decx;
  m_edx = dedx;
 }
