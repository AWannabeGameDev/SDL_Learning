#include "gameloop.h"
#include <stdio.h>
#include <direct.h>

int main(int argc, char* args[]) {

	if(_chdir("../../wd") == -1) printf("Changing working directory failed.\n");

	GameLoop::init();
	GameLoop::run();
	GameLoop::close();

	return 0;

}