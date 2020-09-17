using System;
using System.Net;
using System.Net.Sockets;

namespace IsAlive
{
    class Program
    {
        private static int port = 42;
        private static bool display_pinger = true;

        static void Main(string[] args)
        {
            /* Parse arguments
             * USAGE:
             *
             * -p {port} -d {true/false}
             * -p : Port, -d : Display ping origin
             * 
             */

            int mode = 0;
            foreach (string arg_raw in args)
            {
                string arg = arg_raw.ToLower();

                switch (mode)
                {
                    case 1:
                        try
                        {
                            port = int.Parse(arg);
                        } catch (FormatException) {
                            Console.WriteLine("[FATAL ERROR] Port must be a number!");
                            Exit();
                        }
                        
                        break;

                    case 2:
                        display_pinger = args.Equals("true");
                        break;
                }

                switch (arg)
                {
                    case "-p": // Port
                        mode = 1;
                        break;

                    case "-d": // Display
                        mode = 2;
                        break;

                    default:
                        mode = 0;
                        break;
                }
            }

            Console.Title = $"IsAlive Pinger : Port {port}";
            Console.Clear();

            // Start listening server - Accepts connection then drops it //

            TcpListener server = new TcpListener(IPAddress.Any, port);
            server.Start();

            while (true)
            {
                TcpClient client = server.AcceptTcpClient();  //if a connection exists, the server will accept it

                if (display_pinger)
                {
                    string timeStamp = GetTimestamp(DateTime.Now);
                    Console.WriteLine($"[{client.Client.RemoteEndPoint.ToString().Split(':')[0]}] - [{timeStamp}] Ping request!");
                }
                
                client.Close();
                client.Dispose();
            }
        }
        private static string GetTimestamp(DateTime value)
        {
            return value.ToString("dd.MM.yyyy : HH:mm:ss");
        }

        private static void Exit()
        {
            Console.WriteLine("\nPRESS A BUTTON TO EXIT");
            Console.ReadKey(true);
            Environment.Exit(1);
        }
    }
}
