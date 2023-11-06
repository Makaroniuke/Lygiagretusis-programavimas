using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;
using System.Text;
using System.Text.Json;
using System.Threading;

namespace Lab1a
{
    class Program
    {
        // Sukūrimas - new Thread(fn)
        // Paleidimas - t.Start()
        // Laukimas - t.Join()
        // Būsenos patikrinimas - t.IsAlive()


        private static MyMonitor dataMonitor = new MyMonitor(10);
        private static MyMonitor resultMonitor = new MyMonitor(25);
        static void Main(string[] args)
        {
            if (File.Exists("..\\..\\Rezultatai.txt"))
            {
                File.Delete("..\\..\\Rezultatai.txt");
            }
            Stopwatch sw = new Stopwatch();
            sw.Start();


            // Visi duomenys teisingi
            //List<Plane> planes = ReadWrite.ReadToList("..\\..\\IF01_CapaiteLaura_L1_dat_1.json");

            // Dalis teisinga
            //List<Plane> planes = ReadWrite.ReadToList("..\\..\\IF01_CapaiteLaura_L1_dat_2.json");

            // Nei vienas neteisingas
            List<Plane> planes = ReadWrite.ReadToList("..\\..\\IF01_CapaiteLaura_L1_dat_3.json");





            List<Thread> threads = new List<Thread>();
            
            for (int i = 0; i < 5; i++)
            {
                threads.Add(new Thread(() => Execute()));
                threads[i].Start();
            }
            AddToMonitor(planes);
            

            
            foreach (var thread in threads)
            {
                thread.Join();
            }
            sw.Stop();

            ReadWrite.WriteToFile(planes, "..\\..\\Rezultatai.txt");
            ReadWrite.WriteToFile(resultMonitor, "..\\..\\Rezultatai.txt");

            Console.WriteLine("Program finished execution");

        }

        private static void Execute()
        {
            while(true)
            {
                Plane plane = dataMonitor.Remove();
                if (plane == null)
                {
                    break;
                }
                
                plane.Result = plane.CountFibonacci(plane.Quantity);
                if (plane.Result >= 1000000)
                {
                    resultMonitor.Add(plane);
                }
            }
        }

        public static void AddToMonitor( List<Plane> planes)
        {
            foreach(var plane in planes)
            {
                dataMonitor.Add(plane);
            }
            dataMonitor.SetNotStartAdding(false);
        }






    }
}
