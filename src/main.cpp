#include "GameApp.hpp"

using namespace Beastmaster;

int main(int argc, char** argv)
{
	return GameApp::getInstance().start(1);
}