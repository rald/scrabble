import java.io.*;
import java.util.*;

/**
compdict - a program to construct a dictionary trie for scrabble

Use:
	time java -mx15m CompDict ~/pkg/ospd ospd.trie
	(-mx15m -> allocate 15 MB to the heap)
	
creates the file ospd.trie.  The root is the very last element.
Statistics about nodes and edges in the dawg are reported on
standard output.

(real       27.8
user       15.6
sys         5.7
)
See the file dawg.h for the format of the dawg data structure.

The wordlist MUST be in the form ([a-z]*\n)* and in <B>strictly</B>
increasing lexicographic order, or compdict will weird out.
**/
class CompDict implements DawgConst { 
	public static char[]		buffer;
	public static char[]		newbuffer;
	public static int		string =0;
	
	// accounting
	public static long nodesused = 1;
	public static long edgesused = 1;
	public static long nodessaved = 0;
	public static long edgessaved = 0;

	public static BufferedReader	indata;
	public static DataOutputStream	outdata;
	public static long inlength;
	
	public static Hashtable	table;
	
	public static void
	main(String [] av) throws FileNotFoundException, IOException{
		// input file
		File 	infile = new File(av[0]);
		indata = new BufferedReader(new FileReader(infile));
		inlength= infile.length();
		
		// hash table
		table = new Hashtable((int)inlength, (float)0.25);
		
		// output
		outdata = new DataOutputStream(new FileOutputStream(new File(av[1])));
		// make sure buffer has enough space
		buffer = "**************************************".toCharArray();
	
		convert();
	}

	/** Convert from 'indata' to 'outdata' **/
	public static void
	convert() throws IOException{
		nextword();	// get first word
		outdata.writeInt(0);	// dummy
		int root = makenode (0);
		outdata.writeInt(root);
		System.err.println (
			nodesused + "+" + nodessaved + " nodes;" +
			edgesused + "+" + edgessaved + " edges" +
			root + "+" + "root");
	}

	/**
	   Makenode takes a prefix (as postion relative to stringbuf) and
	   returns an index of the start node of a dawg that recognizes all
	   words beginning with that prefix.  String is a pointer (relative
	   to stringbuf) indicating how much of prefix is matched in the
	   input.
	**/
	public static int
	makenode(int	prefix) throws IOException{
		int		edge;
		int[]	edges = new int[26];
		int		nedges = 0;
	
		while (prefix == string) {		// More edges out of node */
			// advance one character
			edge = buffer[string] = newbuffer[string];
			string++;
			edge -= 'a' -1;
			edge &= CHAR;
	
			if(string == newbuffer.length) {	// end of a word
				edge |= WORD;
				nextword();
			}
			if(!(string<0))
				edge |= makenode(prefix+1);
			edges[nedges++] = edge;
		}
		return storenode(edges, nedges);
	}

	/** Store the next word in 'newbuffer', adjust 'string' **/
	public static void
	nextword() throws IOException{
		String s = indata.readLine();
		if (s==null){
			string = -1;
			return ;
		}
		newbuffer = s.toCharArray();
		buffer[newbuffer.length] = '*';
		string = 0;
		while(buffer[string] == newbuffer[string])
			string++;
		return ;
	}

	public static int
	storenode(int[] edges, int numedges) throws IOException{
		DictNode	n;
	
		if (numedges == 0)
			return 0;		/* Special node zero - no edges */
	
		edges[numedges-1] |= LAST;		/* Mark the last edge */
	
		n = hash (edges, numedges);	/* Look up node in hash table */
		if (n.index != 0) {		// same as an existing node
			edgessaved += numedges;
			nodessaved++;
		} else {			// a new node
			n.index = (int)edgesused;	// index into table
			edgesused += numedges;
			nodesused++;
			outedges(edges, numedges);
		}
		return (n.index << INDEX);
	}

	public static void
	outedges(int[] e, int n) throws IOException{
		for( int j=0; j<n; j++)
			outdata.writeInt(e[j]);
	}
	
		/**
		Hash takes an array of edges (with a count) and maps it to a pointer
		to a node, filling in the edge info if needed.
		**/
	
	public static DictNode
	hash(int[]e, int n) {
	 	DictNode node = new DictNode(e, n);
	 	DictNode existing = (DictNode) table.get(node);
	 	if(existing == null) {
	 		table.put(node,node);
	 		existing  = node;
	 	}
	 	return existing;
	}
}

/**
Class to wrap up an array of edges, mainly to provide
the hashCode() and equals() functions so we can use
them inside a hash table.
**/
class DictNode {
	int	e[];
	int	n;
	int	index;
	static int hashsize;

	public DictNode(int edges[], int nedges) {
		e = new int[nedges];
		System.arraycopy(edges, 0, e, 0, nedges);
		n = nedges;
		index = 0;
	}
	
	public int hashCode() {
		// Cheesy signature method
		int	key = 0;
		for (int i = 0; i < n; i++)
			key ^= (key << 3) ^ (key >> 1) ^ e[i];
		return key;
	}
	
	public boolean equals(Object o) {
		DictNode other = (DictNode) o;
		if(other.n != n)
			return false;
		for(int j=0; j<n; j++)
			if (other.e[j] != e[j])
				return false;
		return true;
	}
	
}
