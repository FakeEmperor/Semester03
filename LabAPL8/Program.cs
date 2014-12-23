using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace LabAPL8
{
    class Program
    {


        static int safeInt(string msg)
        {
            int res = 0;
            string buf;
            bool finished = true;
            do
            {
                finished = true;
                Console.Write(msg);
                buf = Console.ReadLine();
                try
                {
                    res = Int32.Parse(buf);
                }
                catch (Exception e)
                {
                    Console.WriteLine("Invalid input: ");
                    Console.Write(e.Message);
                    finished = false;
                }

            } while (!finished);

            return res;
        }

        public static void AddToDictionaryFromInput(Dictionary<int, int> dict)
        {
            int a = safeInt("Type key: "),
                b = safeInt("Type value: ");
            while (dict.ContainsKey(a))
            {
                Console.WriteLine("Provide another key - key is in use.");
                a = safeInt("Type key: ");
            }
            dict.Add(a, b);
        }

        enum MenuChoice
        {
            UNDEFINED = -1,
            PRINT_ALL = 0,
            ADD_RECORD = 1,
            FIND_RECORD = 2,
            FLUSH_SCREEN = 3,
            EXIT = 4,
        }

        static void UserInput()
        {
            MenuChoice choice = MenuChoice.UNDEFINED;
            Dictionary<int, int> lf = new Dictionary<int, int>();
            MenuChoice[] menu_indexes = { MenuChoice.PRINT_ALL, MenuChoice.ADD_RECORD, MenuChoice.FLUSH_SCREEN, MenuChoice.EXIT, MenuChoice.FIND_RECORD };
            string[] menu_headers = { "Print all records", "Add record", "Flush the screen output", "Close the application", "Find record" };
            int l = menu_headers.Length;
            do
            {
                Console.WriteLine("Main Menu: ");
                for (int i = 0; i < l; ++i)
                    Console.WriteLine((int)menu_indexes[i] + " - " + menu_headers[i]);
                int ch = safeInt("Choose the action: ");
                switch ((MenuChoice)ch)
                {
                    case MenuChoice.PRINT_ALL:
                        Console.Clear();
                        Console.WriteLine("There are " + lf.Count + " figures: ");
                        foreach (var r in lf)
                            Console.WriteLine("Key: " + r.Key.ToString() + ", Value: " + r.Value.ToString());
                        Console.WriteLine("Type something to go back");
                        Console.ReadKey();
                        break;
                    case MenuChoice.FLUSH_SCREEN:
                        Console.Clear();
                        break;
                    case MenuChoice.ADD_RECORD:
                        AddToDictionaryFromInput(lf);
                        break;
                    case MenuChoice.EXIT:
                        return;
                    case MenuChoice.FIND_RECORD:
                        int b = safeInt("Provide key: ");
                        if (lf.ContainsKey(b))
                            Console.WriteLine("Found value: " + lf[b].ToString());
                        else
                            Console.WriteLine("Not found!");
                        break;
                    default:
                        Console.WriteLine("Invalid action");
                        break;
                }

            } while (choice == MenuChoice.UNDEFINED);
        }

        static void Main(string[] args)
        {
            Console.WriteLine("Hi, I'm LabAPL8 - Der'mo kakoe-to.");
            UserInput();
            Console.WriteLine("Application will close now...");
            Thread.Sleep(1500);
        }
    }
}
