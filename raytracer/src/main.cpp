#include <iostream>
#include <ctime>

#include "primitive.h"
#include "scene.h"
#include "tracer.h"
#include "primitivefactory.h"

using namespace std;

Scene *get_default_scene1() {
    Scene *scene = new Scene();
    // Plane
    {
        Primitive *prim = new Plane(Vector3d(0.0f, 1.0f, 0.0f), 4.4f);
        prim->set_name("plane");
        prim->get_material()->set_refl(0.0f);
        prim->get_material()->set_diff(1.0f);
        prim->get_material()->set_color(Color(0.4f, 0.3f, 0.3f));
        scene->add_primitive(prim);
    }
    // Shpere
    {
        Primitive *prim = new Sphere(Vector3d(1.0f, -0.8f, 3.0f), 2.5f);
        prim->set_name("sphere1");
        prim->get_material()->set_refl(0.6f);
        prim->get_material()->set_color(Color(0.7f, 0.7f, 0.7f));
        scene->add_primitive(prim);
    }
    // Sphere2
    {
        Primitive *prim = new Sphere(Vector3d(-5.5f, -0.5f, 7.0f), 2.0f);
        prim->set_name("sphere2");
        prim->get_material()->set_refl(1.0f);
        prim->get_material()->set_diff(0.1f);
        prim->get_material()->set_color(Color(0.7f, 0.7f, 1.0f));
        scene->add_primitive(prim);
    }
    // Light1
    {
        Primitive *light = PrimitiveFactory::create_light_source(
            new Sphere(Vector3d(0.0f, 5.0f, 5.0f), 0.1f));
        light->set_name("light1");
        light->get_material()->set_color(Color(0.4f, 0.4f, 0.4f));
        scene->add_light(light);
    }
    // Light2
    {
        Primitive *light = PrimitiveFactory::create_light_source(
            new Sphere(Vector3d(2.0f, 5.0f, 1.0f), 0.1f));
        light->set_name("light2");
        light->get_material()->set_color(Color(0.6f, 0.6f, 0.8f));
        scene->add_light(light);
    }
    return scene;
}

Scene *get_default_scene2() {
    Scene *scene = new Scene();
    // Floor plane
    {
        Primitive *prim = new Plane(Vector3d(0.0f, 1.0f, 0.0f), 4.4f);
        prim->set_name("plane");
        prim->get_material()->set_refl(0.0f);
        prim->get_material()->set_refr(0.0f);
        prim->get_material()->set_diff(1.0f);
        prim->get_material()->set_color(Color(0.4f, 0.3f, 0.3f));
        scene->add_primitive(prim);
    }
    // Back plane
    {
        Primitive *prim = new Plane(Vector3d(0.4f, 0.0f, -1.0f), 12.0f);
        prim->set_name("plane2");
        prim->get_material()->set_refl(0.0f);
        prim->get_material()->set_refr(0.0f);
        prim->get_material()->set_spec(0.0f);
        prim->get_material()->set_diff(0.6f);
        prim->get_material()->set_color(Color(0.5f, 0.3f, 0.5f));
        scene->add_primitive(prim);
    }
    // Ceil plane
    {
        Primitive *prim = new Plane(Vector3d(0.0f, -1.0f, 0.0f), 7.4f);
        prim->set_name("plane3");
        prim->get_material()->set_refl(0.0f);
        prim->get_material()->set_refr(0.0f);
        prim->get_material()->set_spec(0.0f);
        prim->get_material()->set_diff(0.5f);
        prim->get_material()->set_color(Color(0.4f, 0.7f, 0.7f));
        scene->add_primitive(prim);
    }
    // Sphere
    {
        Primitive *prim = new Sphere(Vector3d(2.0f, 0.8f, 3.0f), 2.5f);
        prim->set_name("sphere1");
        prim->get_material()->set_refl(0.2f);
        prim->get_material()->set_refr(0.8f);
        prim->get_material()->set_refr_rate(1.3f);
        prim->get_material()->set_color(Color(0.7f, 0.7f, 1.0f));
        scene->add_primitive(prim);
    }
    // Sphere2
    {
        Primitive *prim = new Sphere(Vector3d(-5.5f, -0.5f, 7.0f), 2.0f);
        prim->set_name("sphere2");
        prim->get_material()->set_refl(0.5f);
        prim->get_material()->set_refr(0.0f);
        prim->get_material()->set_refr_rate(1.3f);
        prim->get_material()->set_diff(0.1f);
        prim->get_material()->set_color(Color(0.7f, 0.7f, 1.0f));
        scene->add_primitive(prim);
    }
    // Sphere3
    {
        Primitive *prim = new Sphere(Vector3d(-1.5f, -3.8f, 1.0f), 1.5f);
        prim->set_name("sphere3");
        prim->get_material()->set_refl(0.0f);
        prim->get_material()->set_refr(0.8f);
        prim->get_material()->set_color(Color(1.0f, 0.4f, 0.4f));
        scene->add_primitive(prim);
    }
    // Grid
    for (size_t i = 0; i < 8; ++i)
        for (size_t j = 0; j < 7; ++j) {
            Primitive *prim = new Sphere(Vector3d(-4.5f + i * 1.5f,
                -4.3f + j * 1.5f, 10.0f), 0.3f);
                prim->set_name("grid sphere");
                prim->get_material()->set_refl(0.0f);
                prim->get_material()->set_refr(0.0f);
                prim->get_material()->set_spec(0.6f);
                prim->get_material()->set_diff(0.6f);
                prim->get_material()->set_color(Color(0.3f, 1.0f, 0.4f));
                scene->add_primitive(prim);
        }
    // Light1
    {
        Primitive *light = PrimitiveFactory::create_light_source(
            new Sphere(Vector3d(0.0f, 5.0f, 5.0f), 0.1f));
        light->set_name("light1");
        light->get_material()->set_color(Color(0.4f, 0.4f, 0.4f));
        scene->add_light(light);
    }
    // Light2
    {
        Primitive *light = PrimitiveFactory::create_light_source(
            new Sphere(Vector3d(-3.0f, 5.0f, 1.0f), 0.1f));
        light->set_name("light2");
        light->get_material()->set_color(Color(0.6f, 0.6f, 0.8f));
        scene->add_light(light);
    }
    
    return scene;
}

int main(int argc, char **argv) {
    time_t begin, end;
    time_t duration;

    // Init raytraycer
    Tracer *tracer = new Tracer();

    // Init scene
    
    std::cout << "Initialize scene" << std::endl;
    Scene *scene = get_default_scene2(); 
    tracer->set_scene(scene);

    // Init surface
    std::cout << "Initialize surface" << std::endl;
    Surface *surface = new BmpSurface("output.bmp", 800, 600);
    tracer->set_surface(surface);

    std::cout << "Starting rendering" << std::endl;
    begin = clock();
    while (!tracer->render()) {
        
    }
    end = clock();
    duration = (end - begin) / CLOCKS_PER_SEC;
    std::cout << "Finish rendering, cost " << duration << std::endl;

    std::cout << "Storing rendered picture in " <<
        tracer->get_surface()->get_filename() << std::endl;
    tracer->get_surface()->store();

    // Free
    delete tracer;
    
    return 0;
}

