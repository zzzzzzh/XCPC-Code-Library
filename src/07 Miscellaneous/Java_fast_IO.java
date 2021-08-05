public class FastIO {
	public static void main(String[] args){
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		try {
			String string = reader.readLine();
			System.out.println(string);}
		catch (IOException e){throw new RuntimeException(e);}
	}
}
