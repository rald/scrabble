/** Representation of a collection of letters.
 **/
class Letters implements DawgConst {
	// count of how many we have of each letter:
	// 'a' -> 1, 'z'-> 26, 'BLANK' ->27
	int letter[];
	
	static final String name [] = {
		"~",
		"a", "b", "c", "d", "e", "f", "g", "h", "i", 
		"j", "k", "l", "m", "n", "o", "p", "q", "r", 
		"s", "t", "u", "v", "w", "x", "y", "z",
		"?", 
		"*", "*", "*", "*", "*", 
		// Same letters, with BLANKBIT set
		"A", "B", "C", "D", "E", "F", "G", "H", "I", 
		"J", "K", "L", "M", "N", "O", "P", "Q", "R", 
		"S", "T", "U", "V", "W", "X", "Y", "Z",
		"*", "*", "*", "*", "*", "*", "*", "*", 
	};
	
	private static void selfTest(String [] argv) {
		for(int j=0; j< argv.length; j++) {
			String s = argv[j];
			Letters l = new Letters(s);
			System.out.println(s + "->" + l + l.count());
		}
	}
	
	/** main() for testing **/
	public static void
	main(String [] argv) {
		selfTest(argv);
	}
	
	Letters() {
		letter = new int[28];	// all initialized to 0 
	}
	
	/** Construct from a string **/
	Letters(String s) {
		this();
		char[] c= s.toCharArray();
		for(int j=0; j<c.length; j++) {
			int off = (c[j] == '_') ? BLANK : c[j] -'a' + 1;
			letter[off]++;
		}
	}
	
	/** Construct from a bit mask **/
	Letters(int mask) {
		this();
		for(int j=0; j<BLANK; j++) {
			if( (mask & (1<<j)) !=0)
				letter[j]++;
		}
	}
	
	static String
	toString (int[] a, int n) {
		String s ="";
		for(int j=0; j< n; j++)
			s += Letters.name[a[j]];
		return s;
	}
	
	static String
	toString(int[] a) {
		return toString(a, a.length);
	}
	
	public String
	toString() {
		String s = "";
		for(int j=0; j<letter.length; j++) {
			for(int k=0; k< letter[j]; k++) {
				s += name[j];
			}
		}
		return s;
	}
	
	public char[]
	toCharArray() {
		return toString().toCharArray();
	}
	
	int
	count() {
		int total=0;
		for (int j=0 ; j<letter.length; j++) {
			total += letter[j];
		}
		return total;
	}
	static String
	toString(int[][] a, int sort) {
		int width = a.length;
		int height = a[0].length;
		String s = "";
		
		for(int j=0; j< width; j++) {
			String line = "";
			for(int k=0; k< height; k++) {
				int t = a[j][k];
				switch(sort) {
				case ISCOUNT:	
					line += a[j][k]+",";
					break;
				case ISCHAR:
					line += Letters.name[t];
					break;
				case ISMASK:
					Letters l = new Letters(t);
					line += l.toString();
					break;
				default:
					throw new Error("unhandled case");
				}
			}
			s += line + "\n";
		}
		return s;
	}
	
}

	