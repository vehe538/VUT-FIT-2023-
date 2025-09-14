def running_avg(iter):

    for i in range(1, len(iter)):
        if i == 1:
            yield iter[i] / 1
        else:
            s = 0
            for j in range(1, i):
                s += iter[j]
        
            
            yield s / i 
        
    

a = running_avg(['age', 12, 20, 1])

print(list(a))