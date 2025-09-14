from collections import UserDict


class TimeSeriesDB(UserDict):
    
    def __setitem__(self, key, item):
        
        timestamp = item[0]
        value = item[1]

        if key not in self.data.keys():
            self.data[key] = []


        return self.data[key].append((timestamp, value))
    
    def __getitem__(self, key):

        if isinstance(key, tuple):

            k = key[0]
            t = key[1]

            for i in self.data.keys():
                if k == i:
                    for j in range(len(self.data[i])):
                        if self.data[i][j][0] > t:
                            return self.data[i][j-1][1]
        else:
            return self.data[key][-1][1]




time_db = TimeSeriesDB()

time_db['vibr'] = (1, 'low')

time_db['vibr'] = (5, 'mid')

time_db['vibr'] = (8, 'low')

time_db['vibr'] = (12, 'high')

time_db['temp'] = (2, 37.6)

time_db['temp'] = (4, 37.2)

time_db['temp'] = (17, 37.7)


print(time_db[('vibr', 1)])

print(time_db[('vibr', 4)])
print(time_db[('vibr', 7)])
print(time_db['temp'])

print(time_db)

