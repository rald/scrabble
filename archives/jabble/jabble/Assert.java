class Assert {
	public static void
	assert(boolean e) {
		if(!e) throw new Error("assert");
	}
}
