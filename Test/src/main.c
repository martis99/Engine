#include "scene.h"

#ifdef SAPI_WINDOWS
#include <Windows.h>
int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE instance_prev, _In_ LPSTR cmd, _In_ int cmd_n)
{
	return scene_run();
}
#endif

int main(int argc, char **argv)
{
	return scene_run();
}
