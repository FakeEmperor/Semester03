using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace LabAPL5
{
    class Car
    {
        readonly int _min_a = 1, _max_a;
        Random _rand_gen;
        int _speed, _acceleration;
        int _current_distance;
        string _name;
        public int Speed { get { return _speed; } }
        public int Acceleration { 
            get { return _acceleration; }
            set {
                if (value <= this._max_a && value >= this._min_a)
                    _acceleration = value;
                else if (value < this._min_a)
                    _acceleration = this._min_a;
                else
                    _acceleration = this._max_a;
            }
        }
        public int Position { get { return _current_distance;  } }
        public string Name { get { return _name; } }

        public Car(string name, int max_acceleration)
        {
            _current_distance = 0;
            _name = name;
            _max_a = max_acceleration;
            _speed = 0;
            _acceleration = 0;
            _rand_gen = new Random();
        }

        public void Go()
        {
            _acceleration = _rand_gen.Next(_min_a, _max_a + 1);
            _speed += _acceleration;
            _current_distance += _speed;
        }
    }

    class Race
    {
        int distance;
        bool race_finished;

        List<Car> cars;
        List<Car> rank_table;
        static string genCarName(Random r){
            string[] names = {"Alice", "Bob", "Julie", "Mike", "Rose", "Henry"};
            string name = "Car";
            name += " №" + r.Next(200) + " of " + names[r.Next(names.Length)]; 
            return name;
        }

        public bool Finished
        {
            get { return race_finished;  }
        }

        public Race(uint number_of_cars, int distance) {
            this.distance = distance;
            race_finished = false;
            cars = new List<Car>();
            rank_table = new List<Car>();
            Random r = new Random();
            for (uint i=0; i<number_of_cars; ++i)
                cars.Add(new Car(genCarName(r),r.Next(1,4)));
        }

        public void Step()
        {
            
            if (race_finished == false)
            {
                for (int i = cars.Count - 1; i>=0 ; --i)
                {
                    Car c = cars[i];
                    if (c.Position < distance)
                        c.Go();
                    if (c.Position >= distance)
                    {
                        rank_table.Add(c);
                        cars.Remove(c);
                    }

                }    
            }
            if (cars.Count == 0)
                this.race_finished = true;
        }
        public void Statistics()
        {
            Console.Clear();
            foreach (Car c in rank_table)
            {
                Console.WriteLine("FINISHED: {0}", c.Name);
                Console.WriteLine("------------------------------------------------");
            }
                
            foreach (Car c in cars)
            {

                Console.WriteLine("Name: {0}", c.Name);
                Console.WriteLine("Distance From Start: {0:D} Distance From Finsih: {1:D}",
                                  c.Position, this.distance-c.Position);
                Console.WriteLine("Acceleration: {0:N} Speed: {1:N}", c.Acceleration, c.Speed);
                Console.WriteLine("------------------------------------------------");
            }
        }

        public void RankTable( int max_count )
        {
            if (race_finished)
            {
                for(int i =0; i<max_count; ++i)
                    Console.WriteLine("#{0:D} {1:D}", i+1, rank_table[i].Name );
            }
            
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            Race r = new Race(10, 100);
            while (r.Finished == false)
            {
                r.Statistics();
                r.Step();
                Console.ReadKey();
            }
            Console.Clear();
            Console.WriteLine("RANK TABLE: ");
            r.RankTable(3);
            Console.ReadKey();
        }
    }
}
