using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Json;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;

namespace Lab1a

{
    static class ReadWrite
    {
        public static List<Plane> ReadToList(string json)
        {
            string itemsData = File.ReadAllText(json);
            var items = JsonSerializer.Deserialize<List<Plane>>(itemsData);
            return items;
        }

        public static void WriteToFile(List<Plane> planes, string fileName)
        {
            string[] lines = new string[planes.Count() + 4];
            lines[0] = "Pradiniai duomenys:";
            lines[1] = String.Format("| {0, -3} | {1, -25} | {2, 10} | {3, -10} |", "Nr.",
                "Lėktuvo pavadinimas", "Kiekis", "Kaina");
            lines[2] = new string('-', 70);
            for (int i = 0; i < planes.Count(); i++)
            {
                lines[3 + i] = String.Format("| {0, -3} | {1, -25} | {2, 10} | {3, -10} |", i+1,
                planes[i].Name, planes[i].Quantity, planes[i].Price);
            }
            lines[planes.Count() + 3] = "";
            File.AppendAllLines(fileName, lines, Encoding.UTF8);
        }

        public static void WriteToFile(MyMonitor results, string fileName)
        {
            string[] lines = new string[results.GetCount() + 3];
            lines[0] = "Rezultatai:";
            lines[1] = String.Format("| {0, -3} | {1, -25} | {2, 10} | {3, -10} | {4, -10} |", "Nr.",
                "Lėktuvo pavadinimas", "Kiekis", "Kaina", "Rezultatas");
            lines[2] = new string('-', 70);
            for (int i = 0; i < results.GetCount(); i++)
            {
                lines[3 + i] = String.Format("| {0, -3} | {1, -25} | {2, 10} | {3, -10} | {4, -10} | ", i+1,
                results.Get(i).Name, results.Get(i).Quantity, results.Get(i).Price, results.Get(i).Result);
            }
            File.AppendAllLines(fileName, lines, Encoding.UTF8);
        }
    }
}
