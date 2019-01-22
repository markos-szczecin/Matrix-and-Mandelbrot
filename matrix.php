<?php
require_once __DIR__ . '/vendor/autoload.php';

class Matrix
{
    /**
     * @param string $m1
     * @param string $m2
     * @return string
     */
    public function product(string $m1, string $m2): string
    {
        return (string) exec("matrix.exe $m1 $m2");
    }

    /**
     * @param string $name
     * @return string
     */
    public function getProduct(string $name): string
    {
        if (file_exists('matrixProd/' . $name)) {
            return base64_encode(file_get_contents('matrixProd/' . $name));
        } else {
            return '';
        }
    }

    /**
     * @param string $data
     * @return string
     */
    public function addMatrix(string $data)
    {
        $name = 'matrix_' . time();
        file_put_contents($name, base64_decode($data));

        return $name;
    }
}

$serverUrl = "http://localhost/zut/server/matrix.php";
$options = [
    'uri' => $serverUrl,
];
$server = new Zend\Soap\Server(null, $options);

if (isset($_GET['wsdl'])) {
    $soapAutoDiscover = new \Zend\Soap\AutoDiscover(new \Zend\Soap\Wsdl\ComplexTypeStrategy\ArrayOfTypeSequence());
    $soapAutoDiscover->setBindingStyle(array('style' => 'document'));
    $soapAutoDiscover->setOperationBodyStyle(array('use' => 'literal'));
    $soapAutoDiscover->setClass('Matrix');
    $soapAutoDiscover->setUri($serverUrl);
    header("Content-Type: text/xml");
    echo $soapAutoDiscover->generate()->toXml();
} else {
    $soap = new \Zend\Soap\Server($serverUrl . '?wsdl');
    $soap->setObject(new \Zend\Soap\Server\DocumentLiteralWrapper(new Matrix()));
    $soap->handle();
}