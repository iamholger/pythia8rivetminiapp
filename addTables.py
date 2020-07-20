import numpy as np

def checkDistToPoints(DD):
    Yorig  = DD[:,0]
    dYorig = DD[:,1]
    xwidth = DD[:,-2]
    sumw   = DD[:,2]
    sumw2  = DD[:,3]
    area = sumw
    height = area / xwidth
    areaErr = np.sqrt(sumw2)
    heightErr = areaErr / xwidth

    print(np.sum(Yorig - height))
    print(np.sum(dYorig - heightErr))

def distToPoints(sumw, sumw2, xwidth):
    area      = sumw
    height    = area / xwidth
    areaErr   = np.sqrt(sumw2)
    heightErr = areaErr / xwidth

    ret = np.empty((len(area), 2))
    ret[:,0] = height
    ret[:,1] = heightErr
    return ret

def add(AA, BB):
    SF1 = AA[:,-1]
    SF2 = BB[:,-1]

    xwidth = AA[:,-2]

    sumw_1 = AA[:,2]
    sumw_2 = BB[:,2]

    sumw_1 *= 1./SF1
    sumw_2 *= 1./SF2

    sumw = sumw_1 + sumw_2
    sumw*=SF1


    sumw2_1 = AA[:,3]
    sumw2_2 = BB[:,3]

    sumw2_1 *= 1./SF1
    sumw2_2 *= 1./SF2

    sumw2 = sumw2_1 + sumw2_2
    sumw2*=SF1


    return sumw, sumw2, xwidth



if __name__ == "__main__":
    import sys
    D1 = np.loadtxt(sys.argv[1])
    D2 = np.loadtxt(sys.argv[2])

    distToPoints(*add(D1,D2))


