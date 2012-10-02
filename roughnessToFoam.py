#! /usr/bin/env python
# -*- coding: utf-8 -*-

# first draft of roughnessToFoam.py
#
# 1. (at the moment) called from case directory
# 2. runs roughnessToFoam.C --> which writes 0/z0 file (expecting a basic uniform value z0 file with all the boundary patches to exist already)
# 3. copies the terrain_solid z0 nonuniform values to nut[terrain_solid][z0] 

from PyFoam.RunDictionary.ParsedParameterFile   import ParsedParameterFile

# reading z0 file - made with roughnessToFoam.C, from the WAsP map file at the case directory
z0Dict = ParsedParameterFile("0/z0")
z0 = z0Dict["boundaryField"]["terrain_solid"]["value"]
# writing to nut file
nutDict = ParsedParameterFile("0/nut")
nutDict["boundaryField"]["terrain_.*"]["z0"] = z0
nutDict.writeFile()
