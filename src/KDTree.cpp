/*#include "KDTree.h"

template <typename T>
KDTree<T>::KDTree()
{
}

template <typename T>
KDTree<T>::~KDTree() {
    qDeleteAll(tree);
    tree.clear();
}

template <typename T>
T* KDTree<T>::Median(QList<T *> &photons, int axis) {
    QMap<float, T*> list;

    for(int i=0;i<photons.count();i++)
        list[((float*)photons.at(i)->position)[axis]]=photons.at(i);

    return list[list.keys().at(list.keys().count()/2)];
}

template <typename T>
void KDTree<T>::Build(QList<T *> &photons) {
    int size=1;

    while(size<photons.count())
        size*=2;

    tree.resize(size);

    RecurseBuild(photons, 0, 0);
}

template <typename T>
void KDTree<T>::RecurseBuild(QList<T *> &photons, int nodeIndex, int depth) {
    if(photons.count()==0 || nodeIndex>=tree.count())return;

    int axis = depth % 3;

    T* median = Median(photons, axis);

    QList<T*> leftList;
    QList<T*> rightList;

    photons.removeOne(median);
    for(int i=0;i<photons.count();i++) {
        if(((float*)photons.at(i)->position)[axis]<((float*)median->position)[axis])
            leftList.append(photons.at(i));
        else //if(((float*)photons.at(i)->position)[axis]>((float*)median->position)[axis])
            rightList.append(photons.at(i));
    }

    tree[nodeIndex] = median;
    median->divisionAxis = axis;

    RecurseBuild(leftList, 2*nodeIndex+1, depth+1);

    RecurseBuild(rightList, 2*nodeIndex+2, depth+1);
}


template <typename T>
QList<T*> KDTree<T>::FindClosest(const Vector3& point, float maxRadius, int maxPhotons) {
    QList<T*> result;
    QList<HeapNode<T> > heap;

    float maxRadiusSq = maxRadius*maxRadius;

    RecurseSearch(point, maxRadiusSq, maxPhotons, 0, heap);

    for(int i=0;i<heap.size();i++)
       result.append(heap[i].p);

    return result;
}


template <typename T>
void KDTree<T>::RecurseSearch(const Vector3 &point, float& maxRadiusSq, int maxPhotons, int nodeIndex, QList<HeapNode<T> >& result) {
    if(nodeIndex>=tree.count() || tree.at(nodeIndex)==0) return;


    int axis = tree.at(nodeIndex)->divisionAxis;

    float dist = ((float*)point)[axis] - ((float*)tree.at(nodeIndex))[axis];
   // qDebug()<<dist;

    if(dist<0) {
        RecurseSearch(point, maxRadiusSq, maxPhotons, 2*nodeIndex+1, result);

        if(dist*dist<maxRadiusSq)
            RecurseSearch(point, maxRadiusSq, maxPhotons, 2*nodeIndex+2, result);
    }
    else {
        RecurseSearch(point, maxRadiusSq, maxPhotons, 2*nodeIndex+2, result);

        if(dist*dist<maxRadiusSq)
            RecurseSearch(point, maxRadiusSq, maxPhotons, 2*nodeIndex+1, result);
    }

    float distToPhoton = (point-tree.at(nodeIndex)->position).GetSquaredLength();

    if(distToPhoton<maxRadiusSq) {
        HeapNode<T> hn;
        hn.p = tree.at(nodeIndex);
        hn.dist = distToPhoton;


        result.append(hn);

        int index = result.count()-1;
        while(index!=0) {
            int parentIndex = index/2;
            if(result.at(parentIndex)<result.at(index)) {
                HeapNode<T> temp = result.at(index);
                result[index] = result.at(parentIndex);
                result[parentIndex] = temp;
                index = parentIndex;
            }
            else break;
        }

        if(result.count()>=maxPhotons) {
            result[0] = result.last();
            result.removeLast();

            int index = 0;
            while(1) {
                if(2*index+2 >= result.count()) break;

                int largerChild = 2*index+1;
                if(result[2*index+1]<result[2*index+2])
                    largerChild = 2*index+2;

                if(result.at(index)<result[largerChild]) {
                    HeapNode<T> temp = result.at(index);
                    result[index] = result.at(largerChild);
                    result[largerChild] = temp;
                    index = largerChild;
                }
                else break;

            }
            maxRadiusSq = result[0].dist;
        }
    }
}
*/
