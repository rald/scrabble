import java.util.Random;

class Sock extends Letters {
	static Random rand = new Random();
	
	/** Score for each letter **/
	public static byte value[] =
	//  a b c d e    f g h i j       k l m n o p  q r s t u v w x y  z #
	 {0,1,3,3,2,1,  4,2,4,1,8,  5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10,0,
	 // scores for blank posing as each letter
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 };

	static int start_quantity[] =
	//  a b c d  e f g h i j k l m n o p  q r s t u v w x y  z # */
	 {0,9,2,2,4,12,2,3,2,9,1,1,4,2,6,8,2, 1,6,4,6,4,2,2,1,2, 1,2};
	
	Sock () {
		super();
		reset();
	}
	
	/** Reset for a new game **/
	void
	reset() {
		System.arraycopy( start_quantity, 0, letter, 0, letter.length );
	}
	
	/** Pick a random letter from the sock **/
	int
	pick() {
		int n = count();	// how many letters left
		if (n==0)
			throw new Error("pick from empty sock");
			
		// random number between 1 and n
		int r = (Math.abs(rand.nextInt()) % n) + 1;	
		for (int j=1; j< letter.length; j++) {
			if (r <= letter[j]){
				letter[j]--;
				return j;
			} else {
				r -= letter[j];
			}
		}
		throw new Error("n: "+n+" r: "+r);
	}
	
	/** Replace the (possibly null) list of letters back into the sock **/
	void
	replace(int[] letters) {
		// todo 
	}
	
	/** Return the total score for the letters in 'a' **/
	static int
	score(int[]a) {
		int total = 0;
		for(int j=0; j< a.length; j++)
			total +=  value[a[j]&CHAR];
		return total;
	}
}
