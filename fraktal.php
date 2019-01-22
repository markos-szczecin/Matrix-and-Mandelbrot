<?php
require_once __DIR__ . '/vendor/autoload.php';

class Fraktal
{
    /**
     * @param int $size
     * @param int $threads
     * @return string
     */
    public function create(int $size, int $threads): string
    {
        return (string) exec("fraktal.exe $size $threads");
    }

    /**
     * @param string $name
     * @return string
     */
    public function get(string $name): string
    {
        if (file_exists('fraktals/' . $name . '.ppm')) {
            return base64_encode(file_get_contents('fraktals/' . $name . '.ppm'));
        } else {
            return '';
        }
    }
}

$serverUrl = "http://localhost/zut/server/fraktal.php";
$options = [
    'uri' => $serverUrl,
];
$server = new Zend\Soap\Server(null, $options);

if (isset($_GET['wsdl'])) {
    $soapAutoDiscover = new \Zend\Soap\AutoDiscover(new \Zend\Soap\Wsdl\ComplexTypeStrategy\ArrayOfTypeSequence());
    $soapAutoDiscover->setBindingStyle(array('style' => 'document'));
    $soapAutoDiscover->setOperationBodyStyle(array('use' => 'literal'));
    $soapAutoDiscover->setClass('Fraktal');
    $soapAutoDiscover->setUri($serverUrl);
    header("Content-Type: text/xml");
    echo $soapAutoDiscover->generate()->toXml();
} else {
    $soap = new \Zend\Soap\Server($serverUrl . '?wsdl');
    $soap->setObject(new \Zend\Soap\Server\DocumentLiteralWrapper(new Fraktal()));
    $soap->handle();
}