#include "src/rend/Renderer.hpp"
#include "src/Engine.hpp"

int main() {
	
	Engine Aman;
	Aman.Start();

	Renderer Gupta(Aman.getEntitiesPtr());//i will feed the main scene to Renderer so it can access data fast
	Gupta.run([&](float dt) {
		Aman.UpdateLoop(dt);
	});
	
	return 0;
}