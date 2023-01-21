#pragma once
#ifdef _MSC_VER
#pragma section(".CRT$XCU", read)
#define DRIVER2_(f, p)                                           \
	static void f(void);                                     \
	__declspec(allocate(".CRT$XCU")) void (*f##_)(void) = f; \
	__pragma(comment(linker, "/include:" p #f "_")) static void f(void)
#ifdef _WIN64
#define DRIVER(f) DRIVER2_(f, "")
#else
#define DRIVERT(f) DRIVER2_(f, "_")
#endif
#else
#define DRIVER(f)                                         \
	static void f(void) __attribute__((constructor)); \
	static void f(void)
#endif
