/** Print out all words in the dictionary.
 * Usage: java WordList
 * Uses Dawg.java which assumes the dictionary is in ospd.trie
 **/
public class WordList implements DawgConst {

	static byte[] buf = new byte[80];
	static int	pos;

	/** Print out all words in the dictionary. **/
	public static void
	main (String[]argv){
	 	// Wseek to the start node and call print
	 	pos = 0;
	 	int start = wseek (0, pos, Dawg.root);
		print (pos, start);
	}
	
	/**
        * Wseek takes a a start edge  'e' and follows the
        * edges of the dawg pointed to by bytes from 'buf'
        * It returns an edge into the dawg.
	**/
	static int
	wseek (int off, int len, int e)  {
		if (off<len) {
			int node = e >> 8;
			do {
				e = Dawg.dawg[node++];
				if (buf[off] == (e&CHAR))
					return wseek(off+1, len, e);
			} while ((e & LAST) ==0);
			return 0;
		} else
			return e;		// we're done
	}

	/** Print first 'n' bytes of 'buf' **/
	static void print (int n) {
		for(int j=0; j<n; j++)
			System.out.print((char)(buf[j] + 'a' -1));
		System.out.println();
	}

	/**
	* Print takes an offset into buf 'pos' and a start edge 'e' and prints
	* all strings represented by the sub-dawg pointed to by 'e'.
	**/
	static void print (int pos, int e) {
		// If we end a word, print it
		if ((e&WORD)!=0)
			print(pos);

		// Handle the edges (if any) from this node
		int	node = e >> 8;
		if (node!=0) {
			do {
				e = Dawg.dawg[node++];
				buf[pos] = (byte)(e & CHAR);
				print(pos+1, e);		// recur
			} while ( (e&LAST) ==0);
		}	
	}
}
