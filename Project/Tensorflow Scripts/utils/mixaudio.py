import numpy
import Log as l;

# tensor = tensor.numpy() + np.resize(audio1[0:].numpy(), [16000,1])
def sum(list_a, list_b, max_sample):
    c = None
    if len(list_a) < max_sample and len(list_b) < max_sample:
        c = numpy.zeros(max_sample, list_a.dtype)
    else:
        if (len(list_a) > max_sample):
            list_a = list_a[:max_sample]
        if (len(list_b) > max_sample):
            list_b = list_b[:max_sample]
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

__UnitTest(20, 10, 30)
__UnitTest(2, 50, 100)
__UnitTest(122, 50, 100)
__UnitTest(120, 50, 20)
__UnitTest(120, 150, 20)