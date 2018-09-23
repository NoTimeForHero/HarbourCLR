using System.Windows.Forms;
using System.Linq;

namespace MyNamespace
{
    public class Class1
    {
        public static void ShowMsg( string msg )
        {
            MessageBox.Show( "Harbour says: " + msg );
        }

	    public static object[] GetData() {
	        MessageBox.Show("What is going on?");	
	        return null;
	    }

        public static int x5 = 0;

        public static object[] getData2()
        {
            x5++;

            //var result = new int[] {5, 10, 25, 100};
            //var result = new object[] { {5, "KOROVA", 25f };
            var result = new object[]
            {
                (sbyte)25, (short)48, (int)64, (long)155,
                (byte)199, (ushort)50000, (uint)1042, (ulong)2048,
                new object[]
                {
                    5, null, 25f, null,
                    new object[]
                    {
                        255, 255, 255, $"Value_{x5}"
                    },
                    555f, 555f
                },
                16.6f, 32.2d, null,
                //new Label(), new Action(callBack), new ASCIIEncoding()
                //new Label(), new TestStruct(),
            };
            //GCHandle handle = GCHandle.Alloc(result, GCHandleType.Pinned);
            //MessageBox.Show("Pointer to data: " + handle.AddrOfPinnedObject().ToInt64().ToString("X"));
            return result;
        }

        public static void ShowArgs(params object[] objs) {
	        string msg = "";
	        msg += getType(objs);
                MessageBox.Show($"Arguments passed {objs.Length}:\n{msg}", "CLR Hosted App", MessageBoxButtons.OK, MessageBoxIcon.Information);	   
	    }

	    internal static string getType(object[] objs,string prefix="") {
	        string msg = "";
	        for (int i=0;i<objs.Length;i++) {
		    msg += $"{prefix} {i+1} ({objs[i].GetType().Name}) - {objs[i].ToString()}\n";
		    object[] innerObjs = objs[i] as object[];
		    if (innerObjs != null) {
			    msg += getType(innerObjs,prefix + $" {i+1} ->");
		    }	
	        }
	        return msg;	
	    }	

   }

    public class Class2
    {
        public Class2(int a, double b)
        {
            MessageBox.Show($"Created class with a={a}, b={b}");
        }

        public Class2(string message)
        {
            MessageBox.Show($"Created class with message:\n{message}");
        }
    }

    public class Class3
    {
        public int sum;

        public void Add(int sum)
        {
            this.sum += sum;
        }

        public int Result()
        {
            MessageBox.Show($"Total sum: {sum}");
        }
    }
}