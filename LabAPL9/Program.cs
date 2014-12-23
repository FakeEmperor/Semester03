using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Text;
using Utils;

namespace LabAPL9
{
    class Program
    {
        static string IntToString(int val)
        {
            return val.ToString();
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
        static void UserInput()
        {
            MenuChoice choice = MenuChoice.UNDEFINED;
            TreeNode<int> tree = null;
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
                        if (tree != null)
                        {
                            Console.WriteLine("There are " + tree.TreeLength + " records: ");
                            Console.WriteLine(tree.ToString());
                        }
                        else
                            Console.WriteLine("There are none entries yet.");
                        
                        Console.WriteLine("Type something to go back");
                        Console.ReadKey();
                        break;
                    case MenuChoice.FLUSH_SCREEN:
                        break;
                    case MenuChoice.ADD_RECORD:
                        Console.Clear();
                        int val = safeInt("Provide value: ");
                        if (tree != null)
                        {
                            TreeNode<int> selected_tree = tree;
                            int selected_index = 0, tl = tree.TreeLength;
                            Console.WriteLine("Select a node, where to add the value (starting with zero):", val);
                            Console.WriteLine(tree.ToString());
                            do
                                selected_index = safeInt("Provide index: ");
                            while (selected_index < 0 || selected_index >= tl );

                            //select an index
                            Console.WriteLine("Adding value {0} to the node...", val);
                            tree[selected_index].AddChild(val);
                            Console.WriteLine("Value added to the node successfully.");
                        }
                        else
                        {
                            Console.WriteLine("Creating root...");
                            tree = new TreeNode<int>(val);
                            Console.WriteLine("Root created successfully.");
                        }
                        Console.WriteLine("Type something to go back");
                        Console.ReadKey();
                        break;
                    case MenuChoice.EXIT:
                        return;
                    case MenuChoice.FIND_RECORD:
                        Console.Clear();
                        int b = safeInt("Provide a value to search: ");
                        int index = tree.IndexOf(b, true);
                        if (index < 0)
                            Console.WriteLine("Value has not been found");
                        else
                            Console.WriteLine("Value has been found. Index is: {0}", index);

                        Console.WriteLine("Type something to go back");
                        Console.ReadKey();
                        break;
                    default:
                        Console.WriteLine("Invalid action");
                        break;
                }
                Console.Clear();
            } while (choice == MenuChoice.UNDEFINED);
        }
        static void Main(string[] args)
        {
            TreeNode<int>.ToStringFunction = IntToString;
            Console.WriteLine("Hi, I'm LabAPL9 - Demo for generic Tree class.");
            UserInput();
            Console.WriteLine("Application will close now...");
            Thread.Sleep(1500);
        }
    }
}
