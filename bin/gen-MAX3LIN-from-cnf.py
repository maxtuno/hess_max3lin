"""
Generate a Random (and not reduced) MAX3LIN Instance from a standard no 3SAT CNF file
"""

import random
import sys


def sat_creator(vs, ct):
    global dm, rc
    if ct == 1:
        rc.append([vs[0], vs[1], dm])
        dm *= -1

    elif ct == 2:
        for i in range(len(vs)):
            temp = dm
            dm *= -1
            dm += 1
            rc.append([temp, vs[i], dm])
            dm *= -1

    elif ct == 3:
        for i in range(len(vs) - 2):
            temp = dm
            dm *= -1
            dm += 1
            rc.append([temp, vs[i], dm])
            dm *= -1
        rc.append([dm, vs[-2], vs[-1]])
        dm *= -1
        dm += 1


def save_results(file_name):
    global rc
    file2 = open(file_name, 'w')
    file2.write('p lin ' + str(dm - 1) + ' ' + str(len(rc)) + '\n')
    for clause in rc:
        file2.write(str(clause[0]) + ' ' + str(clause[1]) + ' ' + str(clause[2]) + ' {} 0\n'.format(random.choice([+dm, -dm])))
    file2.close()


if __name__ == '__main__':

    print('Generate a Random (and not reduced) MAX3LIN Instance from a standard no 3SAT CNF file')
    print('USAGE: python gen-MAX3LIN-from-cnf.py [*.cnf] [*.max3lin]')

    dm = 0
    rc = []

    ifn = sys.argv[1]
    ofn = sys.argv[2]
    file = open(ifn)
    content = file.read()
    file.close()
    lines = content.split('\n')
    header = True
    nl = 0
    total_lines = len(lines) - 1
    limit = total_lines - 1
    for _ in range(limit, 0, -1):
        if lines[nl] == "":
            total_lines -= 1
        else:
            break

    while nl < total_lines:
        line = lines[nl]
        if line != "" and line[0] == 'c':
            nl += 1
            continue
        else:
            vl = line.split(' ')
            if header:
                vs = int(vl[2])
                dm = vs + 1
                header = False
            else:
                vl.pop()
                tv = len(vl)
                if tv == 3:
                    rc.append([vl[0], vl[1], vl[2]])
                else:
                    if tv == 1:
                        rc.append([vl[0], vl[0], vl[0]])
                    elif tv == 2:
                        rc.append([vl[0], vl[1], dm])
                        rc.append([-dm, vl[0], vl[1]])
                        dm += 1
                    else:
                        fc = vl[:2]
                        sat_creator(fc, 1)

                        mc = vl[2:len(vl) - 2]
                        sat_creator(mc, 2)

                        lc = vl[len(vl) - 2:]
                        sat_creator(lc, 3)

        nl += 1
    save_results(ofn)
