#!/usr/bin/env python3
import pynauty

from pathlib import Path
import os
from os import mkdir
from sys import argv, exit
import shutil

def parse_molecule_nauty(filename):
    f = open(filename, "r")
    lines = f.readlines()
    f.close()

    #Remove empty lines & non-necessary info at beginning of sdf file
    lines = lines[3:]

    #Delimit molecule spec
    found_m = False
    for i, l in enumerate(lines):
        if l[0] in "MAVGS":
            found_m = True
            lines = lines[:i]
            break
    if not found_m:
        raise ValueError("No M Block found")

    #Extract info from mol header and sanity checks
    header = lines[0]
    try:
        nb_vertices = int(header[0:3].strip())
        nb_edges = int(header[3:6].strip())
    except:
        raise ValueError("Invalid header")


    edges = lines[nb_vertices+1:]
    if len(edges) != nb_edges:
        raise ValueError("Invalid number of edges")

    #Construct graph
    g = pynauty.Graph(nb_vertices)
    for edge in edges:
        try:
            edge_begin = int(edge[0:3].strip()) - 1
            edge_end = int(edge[3:6].strip()) - 1
            g.connect_vertex(edge_begin, edge_end)
        except:
            raise ValueError("Invalid edge")
    return g

def get_iso_classes_nauty(indirname):
    inp = Path(indirname)

    reps = []
    classes = []
    id_file = 0
    for f in inp.iterdir():
        g = parse_molecule_nauty(f)
        print(f'File {id_file} {f} ({g.number_of_vertices} vertices) : {len(reps)} classes')
        matches_existing_class = False
        for i,rep in enumerate(reps):
            if pynauty.isomorphic(g,rep):
                matches_existing_class = True
                classes[i].append(f)
                break
        if matches_existing_class == False:
            reps.append(g)
            classes.append([f])
        id_file += 1
    print(f'File {id_file} : {len(reps)} classes')
    return reps, classes

def write_iso_classes(reps, classes, outdir):
    for i, rep in enumerate(reps):
        if i%100 == 0:
            print(f'Class {i}')
        classfilename = outdir+f'/{i+1}'
        mkdir(classfilename)
        for molecule in classes[i]:
            shutil.copy(molecule, classfilename)

        #make representative file
        repfile = open(classfilename+'/rep.txt', "w")
        nb_edges = sum([len(rep.adjacency_dict[k]) for k in rep.adjacency_dict])
        repfile.write(f'{rep.number_of_vertices} {nb_edges}\n')
        for u in rep.adjacency_dict:
            for v in rep.adjacency_dict[u]:
                repfile.write(f'{u} {v}\n')
        repfile.close()

"""
    2 arguments required,
    - name of directory with all input files
    - name of directory to output iso classes
"""
if __name__ == '__main__':
    if len(argv) != 3:
        print(f"Invalid args. Usage is {argv[0]} <input dir> <output dir>")
        exit(1)
    if not os.path.isdir(argv[1]):
        print(f"Supplied input dir ({argv[1]}) does not exist. Exiting...")
        exit(1)

    try:
        mkdir(argv[2])
    except FileExistsError:
        print(f'File {argv[2]} already exists. If you continue, it will be overwritten')
        while True:
            inp = input('Do you wish to continue ? [Y/N]')
            if inp.lower() == 'y':
                shutil.rmtree(argv[2])
                mkdir(argv[2])
                break
            elif inp.lower() == 'n':
                print('Exiting...')
                exit(1)
            print('Answer should be Y or N')
    reps, classes = get_iso_classes_nauty(argv[1])
    write_iso_classes(reps, classes, argv[2])
