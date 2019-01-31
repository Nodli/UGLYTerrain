# Procedural terrain modelisation
BONIS Thomas 11409417
CREVITS François 11609835
RAMBURE Hugo 11808749

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
Code documentation is avilable to compile using the command :
```shell
    doxygen docTerraGen
```
To then open the documentation, open the file __./doc/html/index.html__ in your favourite browser.

# Running the application
It is possible to run three applications.
- the unitary tests using : `./build/test`
- the integrations tests using : `./build/genTerrain`
- the graphical interface using : `./build/genTerrainGraphique`