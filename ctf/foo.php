<?

   date_default_timezone_set('America/Los_Angeles');

   if (file_exists('visitors.log')) {
      $visitors = explode(PHP_EOL, file_get_contents('visitors.log'));
      for ($i=0;$i<count($visitors);$i++){
         list($date, $ip) = explode("\t", $visitors[$i]); 
         if ($ip == $_SERVER[REMOTE_ADDR]) {
            $last = strtotime($date);
            $now = strtotime('now');
            $diff = $now - $last;
            if($diff < 5){
               echo "Brute Forcing NOT PERMITTED!";
               exit;
	    } 
         } 
      }
   }

   $LEETSALT = "9a577f69a783780e";
   $SALTEND = "d3d7505a82e8b493";

   $date = new DateTime();
   $d = $date->format('Y-m-d H:i:s');
   
   $user_id = $_GET['id'];
   $user_pw = $_GET['passwd'];

   $userhash = array();
   $lines = explode("\r\n", file_get_contents('./passwd.txt'));

   for ($i=0;$i<count($lines);$i++){
      list($user, $hash) = explode("\t", $lines[$i]); 

      $userhash[$user] = $hash;

   }

   if (!array_key_exists($user_id, $userhash)) {
      echo "Wrong ID or Password.";
      $line = $d . "\t$_SERVER[REMOTE_ADDR]";
      file_put_contents('visitors.log', $line . PHP_EOL, FILE_APPEND);
      exit;
   }

   $tmp = $LEETSALT . $user_pw . $SALTEND;
   $hashed_pw = md5(md5($tmp));

   if ($hashed_pw != $userhash[$user_id]) {
      echo "Wrong ID or Password.";
      $line = $d . "\t$_SERVER[REMOTE_ADDR]";
      file_put_contents('visitors.log', $line . PHP_EOL, FILE_APPEND);
      exit;
   } else {
      echo "Congratulations! Correct ID/Password Combination";
      exit;
   }
?>
