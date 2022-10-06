from sys import maxsize
import numpy
import random
if __name__ == '__main__':
    import Log as l;
else :
    import utils.Log as l;

# tensor = tensor.numpy() + np.resize(audio1[0:].numpy(), [16000,1])
def sum(list_a, list_b, max_samples):
    c = None
    if len(list_a) < max_samples and len(list_b) < max_samples:
        shape = numpy.shape(list_a)
        if len(shape) > 1 :
            c = numpy.zeros((max_samples, len(shape) -1), list_a.dtype)
        else :
            c = numpy.zeros(max_samples, list_a.dtype)
    else:
        if (len(list_a) > max_samples):
            list_a = list_a[:max_samples]
        if (len(list_b) > max_samples):
            list_b = list_b[:max_samples]
    if len(list_a) < len(list_b):
        if (c is None):
            c = list_b.copy()
        else:
            c[:len(list_b)] += list_b
        c[:len(list_a)] += list_a
    else:
        if (c is None):
            c = list_a.copy()
        else:
            c[:len(list_a)] += list_a
        c[:len(list_b)] += list_b
    
    return c

def resize_random(list_a, size):
    if (len(list_a) >= size):
        return list_a
    shape = numpy.shape(list_a)
    c = None
    if len(shape) > 1 :
        c = numpy.zeros((size, len(shape) -1), list_a.dtype)
    else :
        c = numpy.zeros(size, list_a.dtype)

    rd = random.randrange(size - len(list_a))
    c[rd: rd + len(list_a)] += list_a
    return c


def __UnitTest(list_a_size, list_b_size, max_array_size):
    result = True
    list_a = numpy.empty(list_a_size, float)
    list_b = numpy.empty(list_b_size, float)

    max_length = max_array_size

    for i in range (0, len(list_a)):
        list_a[i] = i

    for i in range (0, len(list_b)):
        list_b[i] = i

    list_c = sum (list_a, list_b, max_length)
    len_c = len (list_c)
    if ( len_c != max_length):
        l.logError("Method sum - Test Failed [Wrong Size]")
        result = False
        exit()

    for x in range(0, max_length):
        a = list_a[x] if x < len(list_a) else 0
        b = list_b[x] if x < len(list_b) else 0
        c = list_c[x]
        if (list_c[x] != a + b):
            l.logError("Method sum - Test Failed [Arrays don't match]")
            result = False
            exit()
    
    if (result):
        l.logGreen("Method sum - Test PASSED!")

def __UnitTest2D(list_a_size, list_b_size, max_array_size):
    result = True
    list_a = numpy.empty((list_a_size, 1), float)
    list_b = numpy.empty((list_b_size, 1), float)

    max_length = max_array_size

    for i in range (0, len(list_a)):
        list_a[i] = i

    for i in range (0, len(list_b)):
        list_b[i] = i

    list_c = sum (list_a, list_b, max_length)
    len_c = len (list_c)
    if ( len_c != max_length):
        l.logError("Method sum - Test Failed [Wrong Size]")
        result = False
        exit()

    for x in range(0, max_length):
        a = list_a[x] if x < len(list_a) else 0
        b = list_b[x] if x < len(list_b) else 0
        c = list_c[x]
        if (list_c[x] != a + b):
            l.logError("Method sum - Test Failed [Arrays don't match]")
            result = False
            exit()
    
    if (result):
        l.logGreen("Method sum - Test PASSED!")

if __name__ == '__main__':
    __UnitTest(20, 10, 30)
    __UnitTest(2, 50, 100)
    __UnitTest(122, 50, 100)
    __UnitTest(120, 50, 20)
    __UnitTest(120, 150, 20)

    __UnitTest2D(12,34,42)