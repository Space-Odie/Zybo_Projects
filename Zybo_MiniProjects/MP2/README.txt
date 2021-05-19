To Run this Project do the following:

1) Open Vivado HLS
2) Create a New Application and import source I2B.c found at Ian_Odonnell_MP2\Source_Files\HLS Project

3) Click on Run Sythesis 

4) Pipelines can be added by clicking on the directive tab. The FOR loops are labeled to easily understand 
	where the constraints are being applied. 
	
5) Click on vivado_hls_compare_report.html located at Ian_Odonnell_MP2\Source_Files\HLS Project
	This will display four different configurations using:
		a) No Constraint
		b) Pipeline 
		c) Unrolling
		d) Latency

6) These directives were applied in the following format
		a) No Constraint - not directives added
		b) Pipeline - Pipeline  (default) was added to main function
		c) unrolling - Unroll was added to the OUTER loop of each function
		d) Latency  -  Latency (min 0; max 1) was added to the INNER loop of each function
		*NOTE: The Latency was applied to the OUTER loop while I was doing my verification video and hence, showed no change. 
		I have since found the warning indicating that I am unable to apply the Latency directive to the OUTER loop and fixed it.
		
			
		
Observations of Each Result:
		a) No Constraint shows a base model of the time required to run through all of the functions
		b) Initiation_Interval Solution (Pipeline) Shows that this constraint, when used on the main functions	
			Can drasticatly bring down the required latency from 22571 to 2186 intervals. 
			However, this came at a cost of requiring more harware than the Zybo-Z710 has. 
		c) Unrolling did not have much of an affect when it was done on just the main function so It was applied to each of the outter loops.
			This allowed for latency to be decreased to 19331 with a much less extensive hardware increase. 
		d) The latency has no effect on this design. 
		
		
		It was clear from this project that different pipelines can be done on different functions in order to meet certain specifications (required latency and hardware constraints)
		However, for this lab, no such timing or hardware specification was given. Because of this, finding the optimal solution could not be done because it is 
		not clear if there is a hardware or performance limit. But in general, a solution should be created that has a similiar hardware increase to timing decrease ratio. 
		