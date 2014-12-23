using System;
using System.Collections.Generic;
using System.Linq;
using System.Collections;
using System.Text;
using System.Threading;
using libZPlay;

namespace LabAPL10
{
    class Program
    {
        
        public class NiggaClass : IFormattable, IEnumerable, ICloneable, IComparable
        {
            public static readonly string[] NiggaNames = {
                        "Chimp" , "Congoid" , "Coon", "Cotton picker" , "Crackhead" ,
                        "Francis", "Kaffir", "Kagemusha", "Taurus", "Plexico",
                        "Alphonso", "Swoops", "Hoopeye", "Gikabe", "Effillyne", "Elizabeth Taylor",
                        "Finis", "Zonah", "Zanthia" ,  "Endrissa"
            };

            List<int> values;
            string name;
            int suppa_rank;

            public string Name {
                get { return this.name; }
            }
            public int SuppaRank {
                get { return this.suppa_rank; }
            }

            public NiggaClass(int suppa_rank, string nigga_name, List<int> values)
            {
                this.values = new List<int>(values);
                this.name = nigga_name;
                this.suppa_rank = suppa_rank;
            }

            public NiggaClass()
            {

                values = new List<int>();
                Random r = new Random();
                suppa_rank = r.Next(100); 
            }

            IEnumerator IEnumerable.GetEnumerator(){
                return values.GetEnumerator();
            }

            string IFormattable.ToString(string format, IFormatProvider provider){
                StringBuilder sb = new StringBuilder();
                sb.AppendFormat("Yo, my name is {0} and I have SuppaRank={1} with Suppa Values: ", this.name, this.suppa_rank );
                sb.AppendLine();
                sb.Append("{");
                foreach (int val in this)
                    sb.Append(val+",");
                sb.Remove(sb.Length - 1, 1);
                sb.Append("}");
                return sb.ToString();
            }

            Object ICloneable.Clone()
            {
                return new NiggaClass(this.suppa_rank, this.name, this.values);
            }

            public int CompareTo(object obj)
            {
                if(obj==null)
                    return -1;
                if (!(obj is NiggaClass))
                    return 1;
                return suppa_rank.CompareTo( (obj as NiggaClass).suppa_rank );
            }

            public static bool operator == (NiggaClass nigga1,NiggaClass nigga2){
                return nigga1!=null && nigga2!=null && nigga1.suppa_rank == nigga2.suppa_rank && nigga1.name == nigga2.name;
            }
            public static bool operator != (NiggaClass nigga1, NiggaClass nigga2){
                return nigga1!=nigga2;
            }

            public static NiggaClass Gen(){
                Random r = new Random();
                int sz = r.Next(10);
                List<int> l = new List<int>();
                for(int i=0; i<sz; ++i)
                    l.Add(r.Next(100));
                return new NiggaClass(r.Next(1000), NiggaClass.NiggaNames[r.Next(NiggaNames.Length)] , l);
            }

        }
        enum MenuChoice
        {
            UNDEFINED = -1,
            ADD_RECORD = 0,
            COMPARE_RECORDS = 1,
            FIND_RECORD = 2,
            PRINT_ALL = 3,
            FLUSH_SCREEN = 4,
            EXIT = 5,
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
                    Console.WriteLine(e.Message);
                    finished = false;
                }

            } while (!finished);

            return res;
        }
        static ManualResetEvent mre = new ManualResetEvent(false);
        static System.Threading.Tasks.Task suppa_task;
        static void SuppaReadKey()
        {
            mre.Reset();
            suppa_task = new System.Threading.Tasks.Task(() =>
            {
                if (!mre.WaitOne(1500))
                    Console.WriteLine("Press some keys, yo!");
                else
                    mre.Reset();
            });
            suppa_task.Start();
            Console.ReadKey();
            mre.Set();
        }
        static void UserInput()
        {
            System.Threading.Tasks.Task task;
            MenuChoice choice = MenuChoice.UNDEFINED;
            List<NiggaClass> suppa_list = new List<NiggaClass>();
            MenuChoice[] menu_indexes = { MenuChoice.ADD_RECORD, MenuChoice.PRINT_ALL, MenuChoice.FLUSH_SCREEN,
                                          MenuChoice.EXIT, MenuChoice.FIND_RECORD, MenuChoice.COMPARE_RECORDS };
            string[] menu_headers = { 
                     "ADD another nigga, nigga", "SHOW ME my gang, brother", "FLUSH the f**g SCREEN, now!",
                     "I WILL QUIT YOU, I WILL F***G QUIT YOU!", "FIND that nigga", "Are you going to COMPARE with me, b**ch?" 
                                    };
            int l = menu_headers.Length;
            do
            {
                Console.WriteLine("What you can do: ");
                for (int i = 0; i < l; ++i)
                    Console.WriteLine((int)(menu_indexes[i]) + " - " + menu_headers[i]);
                int ch = safeInt("What will you do, nigga? ");
                switch ((MenuChoice)ch)
                {
                    case MenuChoice.PRINT_ALL:
                        Console.Clear();

                        if(suppa_list.Count==0)
                            Console.WriteLine("THAT NIGGA HAVE NO NIGGAS");
                        else
                            for(int i =0, s = suppa_list.Count; i<s;++i){
                                NiggaClass nigga = suppa_list[i];
                                Console.Write("Nigga {0}: ", i);
                                Console.WriteLine(nigga);
                            }
                        SuppaReadKey();
                        break;
                    case MenuChoice.FLUSH_SCREEN:
                        break;
                    case MenuChoice.ADD_RECORD:
                        Console.Clear();
                        NiggaClass nig = NiggaClass.Gen();
                        suppa_list.Add(nig);
                        Console.WriteLine("...AND NIGGA WAS MADE...");
                        Console.WriteLine(nig);
                        SuppaReadKey();
                        break;
                    case MenuChoice.EXIT:
                        return;
                    case MenuChoice.COMPARE_RECORDS:
                        Console.Clear();
                        int n1, n2;
                        if (suppa_list.Count > 0)
                        {
                            Console.WriteLine("Your niggas to compare: ");
                            for (int i = 0, s = suppa_list.Count; i < s; ++i)
                            {
                                NiggaClass nigga = suppa_list[i];
                                Console.Write("Nigga {0}: ", i);
                                Console.WriteLine(nigga);
                            }
                            do
                                n1 = safeInt("Nigga 1 number: ");
                            while (n1 < 0 || n1 >= suppa_list.Count);
                            Console.WriteLine(suppa_list[n1]);
                            do
                                n2 = safeInt("Nigga 2 number: ");
                            while (n2 < 0 || n1 >= suppa_list.Count);
                            Console.WriteLine(suppa_list[n2]);
                            int r = suppa_list[n1].CompareTo(suppa_list[n2]);
                            if (r == 0)
                                Console.WriteLine("Them niggas are brothers, yo!");
                            else if (r > 0)
                                Console.WriteLine("Nigga 1 is better than that nigga 2, man");
                            else
                                Console.WriteLine("Nigga 2 is better than that nigga 1, bro");

                        }
                        else
                            Console.WriteLine("YOU HAVE NO NIGGAS TO COMPARE, NIGGA!");
                        SuppaReadKey();
                        break;

                    case MenuChoice.FIND_RECORD:
                        Console.Clear();
                        Console.Write("What is that nigga name, nigga?: ");
                        string nigganame = Console.ReadLine().ToLower();
                        int index = suppa_list.FindIndex( (NiggaClass nigga) => {
                            return nigga.Name.ToLower() == nigganame;
                        });
                        if (index < 0)
                            Console.WriteLine("NIGGA HAS ESCAPED!");
                        else
                        {
                            Console.WriteLine("FOUND THAT NIGGA. NIGGA, HE IS DEAD NOW!");
                            Console.WriteLine("BUT YOU CAN SEE WHAT HIS STATS WERE, NIGGA:");
                            Console.WriteLine(suppa_list[index]);
                            suppa_list.RemoveAt(index);

                        }
                            

                        SuppaReadKey();
                        break;
                    default:
                        Console.WriteLine("Yo, what?");
                        Thread.Sleep(1000);
                        break;
                }
                Console.Clear();
            } while (choice == MenuChoice.UNDEFINED);
        }
        static ZPlay player = new ZPlay();

        static void NiggaMusicPlay(){
            player.OpenFile("nigga.mp3", TStreamFormat.sfAutodetect);
            TStreamTime start = new TStreamTime() , end = new TStreamTime();
            start.ms = 3000;
            TStreamInfo tinfo = new TStreamInfo();
            player.GetStreamInfo(ref tinfo);
            end = tinfo.Length;
            player.PlayLoop(TTimeFormat.tfMillisecond, ref start, TTimeFormat.tfMillisecond, ref end, 1000, true);
        }
        static void NiggMusicStop(){
             player.StopPlayback();
            player.Close();  
        }
        static void Main(string[] args)
        {
            Console.WriteLine("Hi, I'm LabAPL10 - SUPPA NIGGA MEGA CLASS.");
            NiggaMusicPlay();
            Console.WriteLine("THAT NIGGA STOLE MY BIKE!");
            UserInput();
            Console.WriteLine("Application will close now...");
            NiggMusicStop();
            Thread.Sleep(1500);
        }
    }
}
