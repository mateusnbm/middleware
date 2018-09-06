#
#
#

import numpy as np
import statistics

files = [
#    'times_tcp_64_256.txt',
#    'times_tcp_64_1024.txt',
#    'times_tcp_64_51200.txt',
#    'times_tcp_64_102400.txt',
#    'times_udp_64_256.txt',
#    'times_udp_64_1024.txt',
#    'times_udp_64_51200.txt',
#    'times_udp_64_102400.txt'
    'times_tcp_1024_102400.txt',
    'times_tcp_2048_102400.txt',
    'times_tcp_4096_102400.txt',
    'times_udp_1024_102400.txt',
    'times_udp_2048_102400.txt',
    'times_udp_4096_102400.txt',
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
