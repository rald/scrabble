class Mask {
	int	col;
	int	score;
	int	mask;
	
	Mask (int col, int score, int mask){
		this.col = col;
		this.score = score;
		this.mask = mask;
	}
	
	public String toString() {
		return "Mask: " + col + "," + score + "," + Dawg.maskToString(mask);
	}
}
