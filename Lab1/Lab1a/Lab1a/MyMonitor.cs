using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Lab1a
{
    class MyMonitor
    {
        private static readonly object myLock = new object();
        private bool NotStartedAdding = true;
        private Plane[] Planes { get; set; }
        private int Count { get; set; }

        public MyMonitor(int size)
        {
            this.Planes = new Plane[size];
            this.Count = 0;
        }


        public void Add(Plane plane)
        {
            lock (myLock)
            {
                while (this.Count == Planes.Length)
                {
                    Monitor.Wait(myLock);
                }

                int index = FindIndex(plane);
                if (Planes[index] != null)
                {
                    ShiftElement(index);
                }
                Planes[index] = plane;
                this.Count++;
                Monitor.PulseAll(myLock);
            }
        }


        private int FindIndex(Plane item)
        {
            int index = 0;

            for (int i = 0; i < Planes.Length; i++)
            {
                if (Planes[i] == null)
                    break;
                if (Planes[i].CompareTo(item) <= 0)
                    index = i + 1;
                else
                    break;
            }
            return index;
        }

        private void ShiftElement(int index)
        {
            for (int i = Planes.Length - 1; i > index; i--)
            {
                Planes[i] = Planes[i - 1];
            }
            Planes[index] = null;
        }

        public Plane Remove()
        {
            lock (myLock)
            {
                while (this.Count == 0)
                {
                    if (NotStartedAdding)
                    {
                        Monitor.Wait(myLock);                       
                    }
                    else if (this.Count == 0)
                    {
                        Monitor.PulseAll(myLock);
                        return null;
                    }

                }
                Plane item = Planes[this.Count - 1];
                Planes[this.Count - 1] = null;
                this.Count--;
                Monitor.PulseAll(myLock);
                return item;
            }
        }
        

        public Plane Get(int index)
        {
            return Planes[index];
        }

        public int GetCount()
        {
            return this.Count;
        }

        public bool IsStartAdding()
        {        
            return NotStartedAdding;
        }

        public void SetNotStartAdding(bool set)
        {
            lock (myLock)
            {               
                this.NotStartedAdding = set;
                Monitor.PulseAll(myLock);
            }
        }
    }
}
