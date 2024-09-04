
#* select sample rate in Hz here (even division of 500)
SAMPLES_PER_SECOND = 4

# open the csv file
f = open('synthetic-ecg/convert_csv_to_c_array.py', 'r')

# read the file
data = f.read()
# split the data into lines
lines = data.split('\n')

timestamps = []
values = []

#remove line 1 and 2 (header)
lines.pop(0)
lines.pop(0)

#remove last 4 lines (tailer)
lines.pop(-1)
lines.pop(-1)
lines.pop(-1)
lines.pop(-1)

startTime = 1701478914395.001953;
for line in lines:
    # split the line into columns
    columns = line.split(',')

    # add the timestamp to the timestamps list
    timestamps.append(str(round((float(columns[0]) - startTime), 5)))
    # add the value to the values list
    values.append(columns[2])

# convert timestamps from float seconds to int milliseconds
timestamps = [str(int(float(timestamp) * 1000)) for timestamp in timestamps]

# normalize timestamps to occur every 2 ms
for i in range(len(timestamps)):
    timestamps[i] = str(int(i * 2))
    

# extend the data (duplicating data but extend time)
copies = 3;

for i in range(copies):
    timestamps.extend([str(int(int(timestamps[-1]) + 2 * (i + 1))) for i in range(len(timestamps))])
    values.extend(values)


# only take SAMPLES_PER_SECOND samples per second
timestamps = timestamps[::int(500/SAMPLES_PER_SECOND)]
values = values[::int(500/SAMPLES_PER_SECOND)]

# print total entries
length = len(timestamps)

# save the data arrays as a C array of doubles
timestampsString = 'const uint32_t timestamps_' + str(SAMPLES_PER_SECOND) + '[] = {' + ', '.join(timestamps) + '};'
valuesString = 'const float values_' + str(SAMPLES_PER_SECOND) + '[] = {' + ', '.join(values) + '};'

includeGuard = '''#if !defined(__DATA_H__)
#define __DATA_H__

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

#include <stdint.h>
'''

#insert samples per second into the include guard
includeGuard = includeGuard.replace('__DATA_H__', '__DATA_%d_H__' % SAMPLES_PER_SECOND)

includeGuardEnd = '''#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // __DATA_H__
'''

includeGuardEnd = includeGuardEnd.replace('__DATA_H__', '__DATA_%d_H__' % SAMPLES_PER_SECOND)

# open the output file
# filename: data_<SPACING>.h
f = open('data_%dHz.h' % SAMPLES_PER_SECOND, 'w')

# save to output
f.write(includeGuard + '\n')
f.write('#define DATA_LENGTH_' +str(SAMPLES_PER_SECOND) + ' ' + str(length) + '\n')
f.write('#define SAMPLES_PER_SECOND_' +str(SAMPLES_PER_SECOND) + ' ' + str(SAMPLES_PER_SECOND) + '\n')
f.write('#define SPACING_' +str(SAMPLES_PER_SECOND) + ' ' + str(int(1000/SAMPLES_PER_SECOND)) + '\n')
f.write(timestampsString + '\n')
f.write(valuesString + '\n')
f.write(includeGuardEnd + '\n')


