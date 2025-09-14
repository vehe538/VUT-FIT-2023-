# minitask 5

# Modify the body of function store to double ns['n'] and the sizes of the ns['karr'] and ns['varr'] when the number of actually stored
# items (with the addition of the new one) reaches 2/3 of total allocated slots ns['n']
# The last print should result in:
# {'actually_stored': 6, 'n': 16, 'karr': [[], ['name', 'friend'], [], ['age'], ['color'], [], ['id'], [], [], [], [], ['husband'], [], [], [], []], 'varr': [[], ['Alice', 'Bob'], [], [25], ['green'], [], [123], [], [], [], [], ['Emil'], [], [], [], []]}

def setup(ns):
    ns['actually_stored'] = 0
    ns['n'] = 8
    ns['karr'] = [[] for i in range(ns['n'])]
    ns['varr'] = [[] for i in range(ns['n'])]

def store(ns, key, value):

    i = hash(key) % ns['n']
    ns['karr'][i].append(key)
    ns['varr'][i].append(value)
    ns['actually_stored'] += 1

    if ns['actually_stored'] >= ns['n'] * (2/3):  # Check before adding new item
        ns['n'] *= 2

        new_karr = []
        new_varr = []
        for i in range(ns['n']):
            new_karr.append([]) # creating 'karr' and 'varr' lists double the original size
            new_varr.append([])
        
            
        for l in ns['karr']: # mapping values from original lists to new lists based on new hash
            for p in l:
                i = hash(p) % ns['n']
                new_karr[i].append(p)        
        for l in ns['varr']:
            for p in l:
                i = hash(p) % ns['n']
                new_varr[i].append(p)

        
        ns['karr'] = new_karr # change original 'karr' to new
        ns['varr'] = new_varr
               
        
    

        


def lookup(ns, key):
    i = hash(key) % ns['n']
    try:
        j = ns['karr'][i].index(key)
    except ValueError:
        raise KeyError(key)
    return ns['varr'][i][j]

ns1={}
setup(ns1)
store(ns1,'name','Alice')
store(ns1,'age',25)
store(ns1,'color','green')
print(lookup(ns1,'name'))
print(ns1)
store(ns1,'friend','Bob')
store(ns1,'id',123)
print(ns1)
store(ns1,'husband','Emil')
print(ns1)
