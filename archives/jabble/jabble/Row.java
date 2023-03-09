/**
 * A row on the scrabble board.
 * By rotational symmetry, we use a Row to represent columns as well.
 * The most important responsibility of the Row class is to generate
 * new moves.  We incorporate enough information into a row that
 * we can decide what new moves are possible.
 **/

import java.util.*;
import java.awt.Point;

class Row implements DawgConst {
	static final int[] edgeList = Dawg.dawg;
	static final int root = Dawg.root;
	
	// We keep track of the letters already placed in
	// this row, and in the rows above and below,
	// with fill, above and below (respectively).
	// Each element in these arrays is a number from
	// 1 ('a') to 26 ('z') or 1 + BLANKBIT ('blank as a') to ...
	// or 0 (no letter placed there)
	int[] fill;
	int[] above;
	int[] below;
	
	// We have precalculated which letters are OK
	// to place at each position in this row w.r.t. forming
	// words perpendicular to the main word we're forming.
	// i.e. if position 3 is just above the letters "arm", it would
	// be OK to place an 'f' or 'h' at position 3.  The bitmask
	// in cross[3] would reflect that.  If there are no letters
	// above or below a position, the bitmask in 'cross' at
	// that position should be all 1s.
	int[] cross;
	
	// If we do form some perpendicular word, we 
	// need to know the total score of all the old letters
	// used in forming that word.  We keep this info in
	// the array 'score'.
	int[] score;
	
	// Every square on the board has a word-score multiplier
	// and a letter-score multiplier.  For most points on
	// the board this is '1'.
	int[] wordmult;
	int[] lettermult;
	
	// Is this row really a row, (isHorizontal== true) or
	// is it a column?  And which row is it?
	boolean isHorizontal;
	int	row;

	// We keep a reference to the best Move we've seen so far,
	// and modify that Move when we see a better alternative.
	Move	bestSoFar;

	// We generate moves given a particular Rack.
	// In the process of generating various moves, we modify
	// rack by removing letters, but we always put them back
	// again, so by the end of findMove, 'rack' should be
	// as it was.
	Rack	rack;
	
	// As we form possible moves, we keep track of
	//	* how many letters we've grabbed from the rack
	//	* what they are
	//	* where did we put the _first_ letter from the rack
	// Therefore the letter we placed most recently will be 
	// letter[nletters-1]
	int[] letter;
	int	nletters;

	Row (int[] fill, int[] above, int[] below,
		int[] cross, int[] score,
		int[] wordmult, int[] lettermult,
		boolean isHorizontal, int row
		) {
		this.fill = fill;
		this.above = above;
		this.below = below;
		this.cross = cross;
		this.score = score;
		this.wordmult = wordmult;
		this.lettermult = lettermult;
		this.isHorizontal = isHorizontal;
		this.row = row;
		this.letter = new int[7];
	}
	
	public String toString() {
		String s = isHorizontal?"H":"V" +row + ", ";
		for(int j=0; j< fill.length; j++) {
			s += Dawg.letterToString(fill[j]);
		}
		return s;
	}
	
	void startMove(Move m, Rack r) {
		bestSoFar = m;
		rack = r;
		nletters = 0;
	}
	
	/** Search all the possible moves we can place
	 * on this row, with the given Rack.  'm' will be progressively
	 * modified, so that it always contains the best move we've seen
	 * so far.  'r' will be modified as we progress, but should be
	 * back in its original condition when we've finished.
	 **/
	/* In other words findMove is NOT threadsafe.  A parallel version would
	 * be trivial to do, though, just (deep) copy 'r' and use the copy.
	 */
	final void findMove(Move m, Rack r) {
		startMove(m,r);
		
		// An anchor is a square we can use to build
		// from.  It's on a blank square, but next to a non-blank square.
		// We keep track of the last anchor we used, because any words
		// formed from the _next_ anchor won't extend as far back as the
		// _previous_ anchor (or we would have found them when extending
		// forwards from the previous anchor).
		int anchor = 0;
		for(int c = 1; c <= 15; c++) {
			if( fill[c]==0 && (above[c]!=0 || below[c] != 0 || fill[c-1]!=0 || fill[c+1] !=0 )) {
				if(fill[c-1] == 0) {
					// The square behind us is blank.
					// In fact, behind us we have
					// 'c-anchor-1' blank squares which themselves are next to blank
					// squares.  We therefore first generate ALL the word prefixes with
					// at most c-anchor-1 letters.
					prefix(root, c - anchor -1, c);
				} else
					// fill[c-1] != 0
					// This is a square straight after a used square.
					// The _first_ used square of the word stretching
					// back behind us must be at 'anchor+1', because 'anchor'
					// was the last non-filled square
					extend(root, anchor+1);
				anchor = c;
			}
		}
	}

	/** Like findMove, only this is the first move,
	 * so we don't have to start connected to anything.
	 **/
	public void firstMove(Move m, Rack r){
		startMove(m,r);
		extend(root, 8);
	}
	
	/** Generate all the word prefixes with length <= maxprefix,
	 * starting at Dawg edge 'e'.  For every prefix, also try to
	 * extend it to the right from extendCol.
	 **/
	final void
	prefix(int edge, int maxprefix, int extendCol) {
		extend(edge&~TERM, extendCol);
		int node = edge >> INDEX;
		if ( maxprefix ==0 || node == 0)
			return;
		do {
			edge = edgeList[node++];
			int ch = edge&CHAR;
			
			// temporarily pick a tile from the rack
			int tmp = rack.pick(ch);
			if (tmp!=0) {
				letter[nletters++] = tmp;
				prefix(edge, maxprefix-1, extendCol);
				nletters--;
				rack.replace(tmp);
			}
		} while ( (edge&LAST)==0);
	}
	
	/** We've added letters such that we are about to place
	 * our next tile at column 'c', and have just followed 'edge'
	 * in the DAWG.
	 **/
	final void
	extend(int edge, int c) {
		int node = edge >> INDEX;
		int	ech;	// letter on outgoing edge
		
		// character at this column
		int ch = fill[c] &CHAR;	

		// Maybe we've completed a word
		if ( (edge&TERM)!=0 && ch ==0)
			eval (c);
			
		// Go further only if we can proceed in the DAWG
		if (node ==0)
			return;
		
		if (ch ==0) {
			// There IS NOT a letter at c
			// Try all the possible edges from node,
			// making sure they don't stuff up cross-words.
			do {
				edge = edgeList[node++];
				ech = edge&CHAR;
				if ((cross[c] & (1 << ech)) != 0) {
					int tmp= rack.pick(ech);
					if (tmp!=0) {
						letter[nletters++] = tmp;
						extend (edge, c + 1);
						nletters--;
						rack.replace(tmp);
					}
				}
			} while ( (edge&LAST)==0);
		} else {
			// There IS a letter at 'c'.
			// Try to match it with the edges coming
			// from this node.
			do {
				edge = edgeList[node++];
				ech = edge&CHAR;
				if ( ech == ch)
					extend (edge, c + 1);
			} while ( ech <= ch && (edge&LAST)==0);
		}
	}
	
	/** We have completed a word, 'blankColumn' is the blank column directly
	 * after our completed word.  Calculate the score for this word,
	 * and possibly update bestMove.
	**/
	final void
	eval(int blankColumn) {
		Assert.assert(fill[blankColumn] == 0);
		
		// forget it if we haven't placed any letters ourselves
		if (nletters == 0)
			return;

		int 	thisWordMult =1;
		
		// Total score for all the cross words we form
		int	crossWordScores = 0;
		
		// Total points for all the letters we use
		int	totalLettersScore = 0;
		
		// Move backwards through the letters we've placed
		int c= blankColumn;
		for (int lpos = nletters-1; lpos >=0; lpos--) {
			int	existingLetter;

			// Skip over existing letters
			while( (existingLetter = fill[--c]) !=0) {
				totalLettersScore += Sock.value[existingLetter];
			}
			
			// Update scores for placing a letter at c
			int placedLetter = letter[lpos];
			int thisLetterScore = lettermult[c] * Sock.value[placedLetter];
			totalLettersScore += thisLetterScore;
			thisWordMult *= wordmult[c];
		
			// Maybe we also formed a perpendicular word?
			if (score[c] >= 0)
				crossWordScores += (score[c] + thisLetterScore) * wordmult[c];
		}

		int score = crossWordScores + totalLettersScore * thisWordMult;
		
		// Bingo!
		if (nletters==7)
			score += 50;

		if (score > bestSoFar.score) {
			bestSoFar.update(tileVector(blankColumn), score);
		}
	}
	
	/** If 'm' is legal, set m's score, return null.
	 * If 'm' is illegal, return the Problem
	 **/
	public Problem
	checkMove(Move m) {
		// assume that no letters go on top of existing letters
		int leftMost=BOARDSIZE;
		int rightMost = 0;
		int	ch;
		
		int totalLettersScore =0;
		int	crossWordScores = 0;
		int thisWordMult = 1;
		
		// Examine all the tiles we'll be placing
		Enumeration e = m.getTiles();
		while(e.hasMoreElements()) {
			Tile t = (Tile)e.nextElement();
			Point p = t.getPoint();
			ch = t.getLetter();
			int c = getColumn(p);
			
			// track the leftmost and rightmost column
			if (c <leftMost)
				leftMost = c;
			if (c> rightMost)
				rightMost = c;
			
			// ensure this column is free
			if(fill[c] != 0) {
				return new Problem("already a tile here", p);
			}
			
			// ensure we don't make any bad cross-words
			if ((cross[c] & (1 << (ch&CHAR))) == 0) {
				return new Problem("bad cross-move", p);
			}
			
			int thisLetterScore = lettermult[c] * Sock.value[ch];
			totalLettersScore += thisLetterScore;
			thisWordMult *= wordmult[c];
			
			// Maybe we also formed a perpendicular word?
			if (above[c] != 0 || below[c] != 0)
				crossWordScores += (score[c] + thisLetterScore) * wordmult[c];
		}
		
		// Examine all the tiles already existing
		// left of leftmost
		for(int c=leftMost -1; (ch = fill[c]) != 0; c--) {
			totalLettersScore += Sock.value[ch];
		}
		// right of rightmost
		for(int c=rightMost +1; (ch = fill[c]) != 0; c++) {
			totalLettersScore += Sock.value[ch];
		}
		// between leftmost and rightmost
		for(int c=leftMost +1; c<rightMost; c++) {
			if((ch=fill[c])!=0)
				totalLettersScore += Sock.value[ch];
		}
		int score = crossWordScores + totalLettersScore * thisWordMult;
		if (m.isBingo())
			score += 50;
		m.setScore(score);
		return null;
	}
	
	int getColumn(Point p) {
		return isHorizontal? p.x : p.y;
	}
	
	/** Return a Vector of the tiles that would be placed to form
	 * a word.  The letters to be placed are the 'nletters' elements
	 * of 'letter'.  'blankCol' is a blank square immediately after
	 * the word.
	 **/
	final Vector tileVector(int blankCol) {
		Assert.assert(fill[blankCol]==0);
		
		Vector tiles = new Vector(7);
		
		// move backwards through the new letters
		int c = blankCol;
		for (int letterPos = nletters-1; letterPos >=0; letterPos--) {
			// Find the spot where we'll place this letter
			while(fill[--c]!=0)
				;
			Assert.assert(fill[c]==0);
			// where is this place really?
			Point p = isHorizontal? new Point(c, row) : new Point(row, c);
			tiles.addElement(new Tile(p, letter[letterPos]));
		}
		return tiles;
	}
	
	/** Calculate mask information for a square to the
	 * left or right of column 'c' (which must have a letter on it).
	 * 'delta' is -1 or +1 and indicates which direction to move
	 * to find the blank square we'll calculate a mask for.
	**/
	Mask getMask(int c, int delta) {
		Assert.assert(fill[c]!=0);
		Assert.assert(delta == -1 || delta == 1);
		
		// find the blank spot
		while(fill[c]!=0)
			c += delta;
		return getMask(c);
	}
	
	/** Return mask information for blank square at 'column'. **/
	Mask getMask(int column) {
		Assert.assert(fill[column]==0);
		
		// Find the start of the letters immediately before
		// 'column' (if there is one).
		int c= column;
		while(c>0&& fill[c-1]!=0)
			c--;

		// Follow those letters
		int edge = traverse(root, c);
		
		// For every edge from this node, see
		// if following the letters immediately to the right
		// can lead to a full word.
		
		int node = edge>>INDEX;
		if(node==0)
			return new Mask(column,0,0);

		int mask = 0;
		do {
			edge = edgeList[node++];
			if((traverse(edge,column)&TERM) !=0)
				mask |= 1<<(edge&CHAR);
		} while ( (edge&LAST) ==0);
		
		if(mask==0)
			return new Mask(column,0,0);
		
		// Calculate the score
		int score = 0;
		int ch;
		for(c=column-1; c>0 && (ch=fill[c])!=0; c--)
			score += Sock.value[ch];
		for(c=column+1; c<= BOARDSIZE && (ch=fill[c])!=0; c++)
			score += Sock.value[ch];

		return new Mask(column, score, mask);
	}

	/**
	* Traverse from edge e along nodes already on the board,
	* starting at column 'c'.  Return final edge.
	**/
	int traverse(int edge, int c) {
		int ch;
		
	SQUARE:
		while((ch=fill[c]&CHAR) !=0) {
			int node = edge>>INDEX;
			if(node==0)
				return 0;
			do {
				edge = edgeList[node++];
				if ((edge&CHAR)==ch){
					c++;
					continue SQUARE;
				}
			} while((edge&LAST)==0);
			return 0;
		}
		return edge;
	}
}
