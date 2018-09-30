using System.Windows.Forms;
using System.Linq;

namespace MyNamespace
{
    public class Test1
    {
        public static long SumSignedNumbers(int a, long b) {
		return a + b;
	}
	
	public static object[] GetSignedNumbers() {
		return new object[]{
			(byte)120,
			(short)30000,
			(int)5000000,
			(long)2000000000
		};
	}

	public static string StringConcat(string a, string b) {
		return a + " " + b;
	}

	public static int SumArrayElements(object[] elems) {
		int sum = 0;	
		foreach (var item in elems) {
			if (item is int) {
				sum += (int) item;				
			}
			if (item is object[]) {
				sum += SumArrayElements((object[])item);
			}
		}
		return sum;
	}
    }
}