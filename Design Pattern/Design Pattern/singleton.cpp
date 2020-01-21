#include "singleton.h" 

Singleton* Singleton::m_pInstance = NULL;
//该模式如果多次引用单例模式头文件将报错