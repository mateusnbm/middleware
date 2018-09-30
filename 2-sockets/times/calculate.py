#
#
#

import numpy as np
import statistics

files = [
    'times_tcp_4096_49152.txt',
    'times_tcp_4096_98304.txt',
    'times_udp_4096_49152.txt',
    'times_udp_4096_98304.txt'
    ]

for filename in files:

    with open(filename, "r") as filestream:

        for line in filestream:

            numbers = line.split(",")[:-1]
            numbers = [int(x) for x in numbers]
            revised = []

            for number in numbers:

                if number > 0:

                    revised.append(number)

            revised.sort()
            revised = revised[:-1000]
            revised = revised[1000:]

            average = statistics.mean(revised)
            deviation = statistics.stdev(revised)

            print('')
            print(filename)
            print('average ' + str(average))
            print('deviation ' + str(deviation))
            print('max ' + str(max(revised)))
            print('min ' + str(min(revised)))
            print('')

            #print(filename + ' max ' + str(max(revised)))
