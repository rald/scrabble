import java.io.*;

/** Access the Dawg. **/

public class Dawg implements DawgConst {
	static int dawg[];
	static int root;
	static final int SIZEOFINT = 4;
	static final String dict = "ospd.trie";
	
	// static initializer
	static {
		try {
			loadtrie(dict);
		} catch (IOException e) {
			System.err.println("IOException on "+dict);
			System.exit(1);
		}
	}
	
	static String letterToString(int letter) {
		if(letter==0)
			return ".";
		char c = (char)('A' + (letter&CHAR) -1);
		String s = c + (((letter&BLANKBIT)!=0)? "*" : "");
		return s;
	}
	
	static String maskToString(int mask) {
		String s = "";
		for(int j=1; j<=26; j++) {
			if ((mask & 1<<j)!=0)
				s += letterToString(j);
		}
		return s;
	}
	
	public static void
	loadtrie(String name) throws IOException {
		File f = new File(name);
		long length = f.length();
		int	nedges = (int)( length / SIZEOFINT);
		dawg = new int[nedges+1];

		FileInputStream is = new FileInputStream(f);
		BufferedInputStream bis = new BufferedInputStream(is);
		DataInputStream data = new DataInputStream(bis);
		
		System.out.println("reading dictionary...");
		for(int j=0; j<nedges; j++) {
			dawg[j] = data.readInt();
		}
		root = dawg[nedges-1];
		System.out.println("done");
	}
}
