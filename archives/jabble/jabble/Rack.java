import java.util.*;
/** Scrabble 'rack'
 * the letters the player has to choose from.
 **/
class Rack extends Letters implements DawgConst {
	Sock	sock;
	Vector	newLetters;	// letters we picked up last
	
	Rack(String s) {
		super(s);
	}
	
	Rack (Sock s) {
		super();
		newLetters = new Vector(TILES_PER_MOVE);
		sock = s;
		fill();
	}
	public static void
	selfTest() {
		new Rack(new Sock()).test();
	}
	
	public static void
	main(String argv[]) {
		selfTest();
	}
	
	void
	test() {
		do {
			empty();
			fill();
			pickletters();
			System.out.println();
		} while(sock.count()>0);
	}
	
	void
	pickletters() {
		for(int j=1; j<BLANK; j++) {
			int t;
			while((t = pick(j)) != 0){
				System.out.print(((t&BLANKBIT) == 0)? 
							name[t] : 
						"_" + name[t&CHAR] + "_");
			}
		}
	}
	
	/** Try to pick the desired tile from the rack.  Return that tile,
	 ** or a blank masquerading as that tile, or 0.
	 **/
	int
	pick (int want) {
		if (letter[want] > 0) {
			letter[want]--;
			return want;
		} else if (letter[BLANK]>0) {
			letter[BLANK]--;
			return want | BLANKBIT;
		} else {
			return 0;
		}
	}

	/**
	* Remove the list of letters and replace them from the sock.
	 **/
	void
	refresh(Move m) {
		for (Enumeration e = m.getTiles() ; e.hasMoreElements() ;) {
			Tile t = (Tile)e.nextElement();
			int ch = t.getLetter();

			if ((ch & BLANKBIT)!=0) {
				letter[BLANK]--;
			} else {
				letter [ch] --;
			}
		}
		fill();
	}
	
	/** Replace the given tile back into the rack. **/
	void
	replace(int t) {
		if ((t & BLANKBIT)!=0) {
			letter[BLANK]++;
		} else {
			letter [t] ++;
		}
	}
	
	/** Throw away all my letters **/
	void
	empty() {
		for(int j=0; j<letter.length; j++)
			letter[j]=0;
	}
	
	/** Fill the rack from the sock **/
	void
	fill() {
		newLetters.removeAllElements();
		int n = count();
		while(sock.count() > 0 && n < 7) {
			int got = sock.pick();
			newLetters.addElement(new Integer(got));
			letter[got]++;
			n++;
		}
	}
	
	/** Return an Enumeration of letters **/
	Enumeration getNewLetters() {
		return newLetters.elements();
	}
}
