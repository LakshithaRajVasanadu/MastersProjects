import java.io.*;
import java.util.*;
import java.lang.Math.*;

public class NB {
	/*public int run(String[] args) throws Exception
	  {*/
	/*JobConf conf = new JobConf(getConf(), Driver.class);
	  JobConf conf1 = new JobConf(getConf(), Driver.class);
	  conf.setJobName("SpamCount");
	  conf1.setJobName("HamCount");

	  conf.setOutputKeyClass(Text.class);
	  conf1.setOutputKeyClass(Text.class);
	  conf.setOutputValueClass(Text.class);
	  conf1.setOutputValueClass(Text.class);

	  conf.setMapperClass(Mapper1.class);
	  conf1.setMapperClass(Mapper1.class);
	  conf.setReducerClass(Reducer1.class);
	  conf1.setReducerClass(Reducer1.class);

	//FileInputFormat.setInputDirRecursive(conf,true);

	FileSystem fs= FileSystem.get(conf); 
	FileStatus[] status_list = fs.listStatus(new Path(args[0]));
	if(status_list != null){
	for(FileStatus status : status_list){
	FileInputFormat.addInputPath(conf, status.getPath());
	}
	}
	FileSystem fs1= FileSystem.get(conf1); 
	FileStatus[] status_list1 = fs1.listStatus(new Path(args[2]));
	if(status_list1 != null){
	for(FileStatus status : status_list1){
	FileInputFormat.addInputPath(conf1, status.getPath());
	}
	}

	//FileInputFormat.addInputPath(conf1, new Path(args[2]));
	FileOutputFormat.setOutputPath(conf, new Path(args[1]));
	FileOutputFormat.setOutputPath(conf1, new Path(args[3]));


	conf.setNumReduceTasks(1);
	conf1.setNumReduceTasks(1);
	JobClient.runJob(conf);
	JobClient.runJob(conf1);

	return 0;
	}
	*/
	public static void main(String[] args) throws Exception {
		//System.out.println("\n Ham Files found : "); 
		final long startTime = System.currentTimeMillis();
		Map<String,Integer> mapSpamWords= new HashMap<String,Integer>();
               
		Map<String,Integer> mapHamWords= new HashMap<String,Integer>();
		StringBuffer stringBuffer = new StringBuffer();
               File folder = new File("/HADOOP/hdfs/user/bigdata04/srinivas/enron2GB_spam_output1/");
		File[] listOfFiles = folder.listFiles();

		String line;
for (File file : listOfFiles) {
    
	                	System.out.println(file+"\n" ); 

		BufferedReader in = new BufferedReader( new FileReader(file));
		while((line = in.readLine()) != null)
		{
			if(line != null) {
				line = line.replaceAll("\t", ",");
				String[] values = line.split(",");
				String ID = values[0];
				Integer numDoc = Integer.parseInt(values[1]);
                                if(mapSpamWords.containsKey(values[0]))
				mapSpamWords.put(ID,numDoc+mapSpamWords.get(values[0]));
                                else 
				mapSpamWords.put(ID,numDoc);
                                
	                	//System.out.println(line+"\n" ); 
			}
		}
}

               folder = new File("/HADOOP/hdfs/user/bigdata04/srinivas/enron2GB_ham_output1/");
               listOfFiles = folder.listFiles();

for (File file : listOfFiles) {
               
	                	System.out.println(file+"\n" ); 

		BufferedReader in1 = new BufferedReader( new FileReader(file));
		while(((line = in1.readLine()) != null))
		{   
			if(line != null) {
				line = line.replaceAll("\t", ",");
				String[] values = line.split(",");
				String ID = values[0];
				Integer numDoc = Integer.parseInt(values[1]);
                                if(mapHamWords.containsKey(values[0]))
                                mapHamWords.put(ID,numDoc+mapHamWords.get(values[0]));
                                else
				mapHamWords.put(ID,numDoc);
			}
		}
}
		int numSpamFiles=0;
		int numHamFiles=0;
		int TotalFiles=0;
		folder = new File("/HADOOP/hdfs/user/bigdata04/enron50/spam_testing");
		listOfFiles = folder.listFiles();
		if(listOfFiles == null) return;  // Added condition check
		for (File file : listOfFiles) {
			String path = file.getPath().replace('\\', '/');
			System.out.println(path);
			TotalFiles=TotalFiles+1;


			List<String> mapNew = new ArrayList<String>();
			BufferedReader newfile = new BufferedReader( new FileReader(path));
			StringBuffer stringBuffer1 = new StringBuffer();
			String freshLine = "";
			while(((freshLine = newfile.readLine()) != null)) {
				stringBuffer1.append(freshLine);
				freshLine = freshLine.toString();
				if(freshLine != null) {
					freshLine = freshLine.toLowerCase();
					freshLine = freshLine.replaceAll("<.*?>", "");
					freshLine = freshLine.replaceAll("[^a-z \\s]", "");
					freshLine = freshLine.trim();
				}
				StringTokenizer tokenizer = new StringTokenizer(freshLine);
				ArrayList<String> b = new ArrayList<String>();
				while (tokenizer.hasMoreTokens()) {
					String buffer = tokenizer.nextToken();
					b.add(buffer);
				}
				for(int i = 0; i < b.size(); i++){
					if(!mapNew.contains(b.get(i))) {
						mapNew.add(b.get(i));
					}
				}
			}

			Collections.sort(mapNew);
			double nuw=1.0;
			double nuw1=1.0;
			//double p1[] = new double [2000];
			double p1=1;
			double p2 =1;
			double num = 0.0;
			double num1 = 0.0;
			double den = 1.0;
			double den1 = 1.0;
			int totalSpamDocs = 11517;
			int totalHamDocs = 11200;
			for(int l=0;l<mapNew.size();l++){

				//		System.out.println("\n probability word is  : "+ mapNew.get(l)); 
				if((mapSpamWords.get(mapNew.get(l)) != null)) {
					if((mapHamWords.get(mapNew.get(l)) != null)) {
						num = ((double)(mapSpamWords.get(mapNew.get(l)))/(double)(totalSpamDocs)) ;
						den = ( (double)num+(((mapHamWords.get(mapNew.get(l))))/(double)(totalHamDocs)));
						num1 = ((double)(mapHamWords.get(mapNew.get(l)))/(double)(totalHamDocs)) ;
						den1= ( (double)num1+(((mapSpamWords.get(mapNew.get(l))))/(double)(totalSpamDocs)));
					}
				}
				if((mapSpamWords.get(mapNew.get(l)) == null)&&(mapHamWords.get(mapNew.get(l)) != null)) 
				{ num=0; den=1; num1=1;den1=1;}

				if((mapHamWords.get(mapNew.get(l)) == null)&&(mapSpamWords.get(mapNew.get(l)) == null) )
				{ continue;}
				if((mapSpamWords.get(mapNew.get(l)) != null)&& (mapHamWords.get(mapNew.get(l)) == null)){
					num1=0;den1=1;num=1;den=1;
				}
				p1= num/den;
				p2=num1/den1;

				if(p1 >1 || p2 >1){
					System.out.println("\n probability with > 1 : "+p1 + " "+p2 ); 
				}
				if(p1==0) p1=0.001;
				if(p2==0) p2=0.001;
				if(p1==1) p1=0.999;
				if(p2==1) p2=0.999;
				if((mapSpamWords.get(mapNew.get(l)) != null)&& (mapHamWords.get(mapNew.get(l)) != null)) 
					//		System.out.println("\n probability words spam: and words ham: "+(mapSpamWords.get(mapNew.get(l))) + " "+(mapHamWords.get(mapNew.get(l))) ); 
					//		System.out.println("\n probability num and den and probability: "+num + " "+den+" "+p1[l] ); 
					//	nuw+= Math.log(p1[l])- Math.log(1-p1[l]);
					nuw = (nuw*(1.0-p1))/p1;
				nuw1 = (nuw1*(1.0-p2))/p2;
				//		System.out.println("\n probability nuw that : "+nuw ); 
				//		System.out.println("\n probability nuw1 that : "+nuw1); 
			}
			//	double probSpam=(1.0)/(1.0+Math.pow(Math.E,nuw));
			double probSpam=(1.0)/(1.0+nuw);
			double probHam=(1.0)/(1.0+nuw1);
			System.out.println("\n probability that it is a spam is : "+numSpamFiles ); 
			System.out.println("\n probability that it is a Ham is : "+numHamFiles ); 
			if(probSpam > probHam) numSpamFiles+=1;
			else numHamFiles+=1;
		}
		final long duration = System.currentTimeMillis() - startTime;
		System.out.println("time for the execution is :"+duration+"ms");
			
		System.out.println("\n spam Files found: "+numSpamFiles ); 
		System.out.println("\n Ham Files found : "+numHamFiles ); 
	}
}
