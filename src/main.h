#ifndef MAIN_H
#define MAIN_H

#ifndef UTEST
#define MAIN main
#define LOOP while(1)
#else
#define MAIN under_test_main
#define LOOP 
#endif

int MAIN(void);

#endif // MAIN_H