//File: main.cpp

//Program: SDL_HelloWorld

#include <iostream>
#include "map.h"
using std::cout;
using std::endl;

//SDL stuff.
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

//GUI stuff
#include <TGUI/TGUI.hpp>

//OpenCL stuff
#include "kernelProgram.h"
#include "simulation.h"

#include "randomNumberGenerator.h"

#include "oldClasses.h"

int main(int argc, char *args[])
{
    ClDevice device;

    std::cout << device.getDevice().name() << std::endl;

    KernelProgram kP1(device, "kernels/neurons.cl", "neuronCalc01");

    cout << "Hello kenrel!\n";

    Simulation sim;

    sf::RenderWindow window(sf::VideoMode(1600, 1200), "CreatureSim");

    tgui::GuiSFML gui{window};

    gui.loadWidgetsFromFile("resources/form.txt");
    auto slider = gui.get<tgui::Slider>("PopSizeSlider");
    auto popCounter = gui.get<tgui::Label>("PopSizeCounter");
    auto submitButton = gui.get<tgui::Button>("ConfirmButt");
    auto configPanel = gui.get<tgui::Panel>("ConfigPanel");

    std::vector<obsoleteSpecimen> species = {};
    std::vector<obsoleteNeuron> neurons = {};
    std::vector<float> results = {};
    std::vector<sf::CircleShape> circles = {};
    compute::vector<float> device_species(device.getContext());
    compute::vector<float> device_neurons(device.getContext());
    compute::vector<float> device_results(device.getContext());

    int creatureNum;
    bool agreed = false;

    submitButton->onPress([&] {
        creatureNum = (int)slider->getValue();
        // sim.addCreatures(creatureNum);
        std::cout << creatureNum;
        configPanel->setVisible(false);
        configPanel->setEnabled(false);
        circles.resize(creatureNum);
        for (int i = 0; i < creatureNum; i++)
        {
            species.emplace_back(1600.0f, 1200.0f);
            neurons.emplace_back();
            neurons.emplace_back();
            circles[i].setFillColor(sf::Color(species[i].r, species[i].g, species[i].b));
            circles[i].setRadius(10.f);
        }
        device_species.resize(creatureNum * 6);
        device_neurons.resize(creatureNum * 6 * 2);
        device_results.resize(creatureNum * 2);
        results.resize(creatureNum * 2);
        agreed = true;
    });

    sf::Clock clock;

// int main (int argc, char* argv[]) {
    int width = 1600;
    int height = 1200;
    // sf::RenderWindow window(sf::VideoMode(width, height), "CreatureSim");
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
    Map map(width, height);
    unsigned char * pixels = map.generateR();
    image.create(width,height,pixels);
    texture.create(width,height);
    texture.update(image);
    sprite.setTexture(texture);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        gui.handleEvent(event);
        window.clear();
        gui.draw();
        if (agreed)
        {
            std::vector<float> tmpSpecies = {};
            for (auto v : species)
            {
                // tmpSpecies.emplace_back(v.xPos);
                // tmpSpecies.emplace_back(v.yPos);
                // tmpSpecies.emplace_back(v.size);
                tmpSpecies.emplace_back(1);
                tmpSpecies.emplace_back(random<float>(-1, 1));
                tmpSpecies.emplace_back(-1);
                tmpSpecies.emplace_back(v.age / 1000.f);
                tmpSpecies.emplace_back(v.speed / (sqrt(1600 * 1600 + 1200 * 1200) / 500.0f));
                tmpSpecies.emplace_back(v.hdg / (2.0f * 3.14159f));
            }

            std::vector<float> tmpNeurons = {};
            for (auto v : neurons)
            {
                tmpNeurons.emplace_back(v.w1);
                tmpNeurons.emplace_back(v.w2);
                tmpNeurons.emplace_back(v.w3);
                tmpNeurons.emplace_back(v.w4);
                tmpNeurons.emplace_back(v.w5);
                tmpNeurons.emplace_back(v.w6);
            }

            compute::copy(
                tmpSpecies.begin(), tmpSpecies.end(), device_species.begin(), device.getQueue());
            auto neuronsPointer = neurons.data();
            auto floatsNeurons = reinterpret_cast<float*>(neuronsPointer);


            compute::copy(
                floatsNeurons, floatsNeurons+neurons.size()*6, device_neurons.begin(), device.getQueue());

            kP1.getKernel().set_args(creatureNum, 6, 2, 1, device_species.get_buffer(), device_neurons.get_buffer(), device_results.get_buffer());
            device.getQueue().enqueue_1d_range_kernel(kP1.getKernel(), 0, creatureNum, 1, device.getWaitlist());

            compute::copy(
                device_results.begin(), device_results.end(), results.begin(), device.getQueue());

            window.draw(sprite);
            
            for (int i = 0; i < creatureNum; i++)
            {
                // std::cout<<"Wynik: "<<results[2*i]<<" "<<results[2*i+1]<<"\n";
                species[i].speed += 0.01f * results[2 * i];
                if (species[i].speed > sqrt(1600 * 1600 + 1200 * 1200) / 500.0f)
                {
                    species[i].speed = sqrt(1600 * 1600 + 1200 * 1200) / 500.0f;
                }
                if (species[i].speed < -sqrt(1600 * 1600 + 1200 * 1200) / 500.0f)
                {
                    species[i].speed = -sqrt(1600 * 1600 + 1200 * 1200) / 500.0f;
                }
                species[i].hdg += 0.1f * results[2 * i + 1]; // * 3.14159f;
                if (abs(species[i].hdg) > 2.0f * 3.14159f)
                {
                    species[i].hdg -= 2.0f * 3.14159f;
                }
                if (abs(species[i].hdg) < 0)
                {
                    species[i].hdg += 2.0f * 3.14159f;
                }
                species[i].xPos += sin(species[i].hdg) * species[i].speed;
                if (species[i].xPos > 1600)
                {
                    species[i].xPos -= 1600;
                }
                if (species[i].xPos < 0)
                {
                    species[i].xPos += 1600;
                }
                species[i].yPos += cos(species[i].hdg) * species[i].speed;
                if (species[i].yPos > 1200)
                {
                    species[i].yPos -= 1200;
                }
                if (species[i].yPos < 0)
                {
                    species[i].yPos += 1200;
                }

                species[i].age += 1.0f;

                circles[i].setPosition(species[i].xPos, species[i].yPos);
                // circles[i].setFillColor(sf::Color(
                //     (int)species[i].xPos - ((int)(species[i].xPos) / 256) * 256,
                //     (int)species[i].yPos - ((int)(species[i].yPos) / 256) * 256,
                //     200));
                window.draw(circles[i]);
            }
        }
        popCounter->setText(std::to_string((int)slider->getValue()));

        sf::Time elapsed1 = clock.getElapsedTime();
        sf::Time milli16 = sf::milliseconds(16);
        if (elapsed1 < milli16)
            sf::sleep(milli16 - elapsed1);
        clock.restart();
        window.display();
    }

    return 0;
};
