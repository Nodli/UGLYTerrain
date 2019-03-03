# Udoubtedly GLorYous Terrain

This is a C++ project done for the "Modeling for Videogames" subject during my Master in Image, Development and 3D Technologies. The goal was to implement procedural terrain generation methods.

The application generates a terrain using a Perlin Noise on which we apply thermal and hydaulic erosion models. The terrain is then populated with fictive vegetal species depending on their needs and the local terrain parameters such as humidity and light exposure.

Three people worked on this project: BONIS Thomas, CREVITS François and myself, Hugo RAMBURE. My principal contribution to this project is the modeling of the thermal erosion of terrains. A layer-based erosion model was implemented during the final iteration to show that the terrain topography is dependent on the underground geological structures.

## Results
The report and defense PDF files are available in ```results/```.
![Alt text](/result/swiss_photo.jpg?raw=true "Picture taken in the Swiss Alps, [Geosciences Montpellier, J. Malavieille, insu.cnrs.fr]")
![Alt text](/result/swiss_sim.png?raw=true "A possible result of our layered thermal erosion model")

![Alt text](/result/jura_photo.jpg?raw=true "Picture taken in the Jura Mountains, [Laboratoire de Geosciences de Lyon / ENS Lyon, Pierre Thomas, planet-terre.ens-lyon.fr]")
![Alt text](/result/jura_sim.png?raw=true "A possible result of our layered thermal erosion model")

## How to compile the code
Most of the functionnalities are linux only.
In order to build the projet run the following commands:
```shell
    mkdir build
    cd build
    cmake ..
    make
```

## Documentation
Code documentation is available to compile using the command :
```shell
    doxygen docTerraGen
```
To then open the documentation, open the file __./doc/html/index.html__ in your favourite browser.

# Running the application
It is possible to run three applications.
- the unitary tests using : `./build/test`
- the integrations tests using : `./build/genTerrain`
- the graphical interface using : `./build/genTerrainGraphique`
